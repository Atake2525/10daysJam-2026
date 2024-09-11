#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "imgui.h"
#include "kMath.h"
#include "numbers"

/// <summary>
/// 変更箇所の説明
/// </summary>
/// ステージ変更アニメーションのfloat destinationRotationZTableの中の角度をステージを追加するごとに変更すること
/// 角度は自由Z軸にしか回転しない
/// ※現在調整中



GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	Stage01Model_ = Model::CreateFromOBJ("Stage01");
	SelectCubeModel_ = Model::CreateFromOBJ("StageSelectCube");
	worldTransform_.Initialize();
	cubeWorldTransform_.Initialize();
	worldTransform_.scale_.x = 15.0f;
	worldTransform_.scale_.y = 15.0f;
	viewProjection_.Initialize();
	viewProjection_.rotation_.x = 0.5f;
	viewProjection_.translation_.y = 5.0f;

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	ChangePhase();
	//プレイヤーのフェーズごとの処理は触っていません。

	// 特定のキーを入力したら選択されているステージが変わる
	if (Input::GetInstance()->TriggerKey(DIK_LEFT) && LeftTurnFlag == false && RightTurnFlag == false) {
		selectStage_ -= 1;
		nowPosition = worldTransform_.translation_;
		LeftTurnFlag = true;
	}
	if (Input::GetInstance()->TriggerKey(DIK_RIGHT) && LeftTurnFlag == false && RightTurnFlag == false) {
		selectStage_ += 1;
		nowPosition = worldTransform_.translation_;
		RightTurnFlag = true;
	}
	if (selectStage_ <= 0) {
		selectStage_ = StageNum;
	} else if (selectStage_ > StageNum) {
		selectStage_ = 1;
	}
	// 特定のキー入力でステージを決定
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && LeftTurnFlag == false && RightTurnFlag == false) {
		finished_ = true;
	}
	/// <summary>
	/// ステージ変更アニメーション
	/// </summary>
	if (LeftTurnFlag) {
		turnTimer_ += 1 / turntime_ / 60;
		float destinationRotationZTable[] = {
		    0,
		    std::numbers::pi_v<float>,
		};
		// 状態に応じた角度を取得
		float destinationRotationZ = destinationRotationZTable[static_cast<uint32_t>(selectStage_) - 1];
		worldTransform_.translation_.x = easeInOut(turnTimer_, worldTransform_.translation_.x, 10.0f);
		worldTransform_.translation_.y = easeInOut(turnTimer_, worldTransform_.translation_.y, 10.0f);
		// 球体の角度設定
		

		cubeWorldTransform_.rotation_.z = easeInOut(turnTimer_, cubeWorldTransform_.rotation_.z, destinationRotationZ);
		if (turnTimer_ > 1.0f) {
			LeftTurnFlag = false;
			turnTimer_ = 0.0f;
		}
	} else if (RightTurnFlag) {
		turnTimer_ += 1 / turntime_ / 60;
		float destinationRotationZTable[] = {
		    0,
		    std::numbers::pi_v<float>,
		};
		// 状態に応じた角度を取得
		float destinationRotationZ = destinationRotationZTable[static_cast<uint32_t>(selectStage_) - 1];
		cubeWorldTransform_.rotation_.z = easeInOut(turnTimer_, cubeWorldTransform_.rotation_.z, destinationRotationZ);
		if (turnTimer_ > 1.0f) {
			RightTurnFlag = false;
			turnTimer_ = 0.0f;
		}
		
	}

	ImGui::Begin("test");
	ImGui::DragFloat3("Rotation", &cubeWorldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("Camerarotation", &viewProjection_.rotation_.x, 0.1f);
	ImGui::DragFloat3("CameraTranslation", &viewProjection_.translation_.x, 0.1f);
	ImGui::End();

	worldTransform_.rotation_ = cubeWorldTransform_.rotation_;

	viewProjection_.UpdateMatrix();
	cubeWorldTransform_.UpdateMatrix();
	cubeWorldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	Stage01Model_->Draw(worldTransform_, viewProjection_);
	SelectCubeModel_->Draw(cubeWorldTransform_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsClear()) {
			phase_ = Phase::kClear;
		}
		if (player_->IsDead()) {
			phase_ = Phase::kDeath;
		}
		break;
	case Phase::kClear:
		finishedClear_ = true;
		break;
	case Phase::kDeath:
		finishedDead_ = true;
		break;
	}
}
