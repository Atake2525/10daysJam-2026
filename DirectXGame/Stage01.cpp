#include "Stage01.h"
#include "TextureManager.h"
#include "imgui.h"
#include "kMath.h"
#include <cassert>

void Stage01::GenerateBlocks() {
	// 要素数
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

Stage01::Stage01() {}

Stage01::~Stage01() {
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete player_;
	delete mapChipField_;
	for (int i = 0; i < 200; i++) {
		delete fallRock_[i];
	}
}

void Stage01::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/Stage01.csv");
	blockModel_ = Model::CreateFromOBJ("glassFloor");

	GenerateBlocks();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 19);
	player_ = new Player;
	modelPlayer_ = Model::CreateFromOBJ("player");
	player_->Initialize(modelPlayer_, &viewProjection_, {playerPosition});
	player_->SetMapChipField(mapChipField_);

	player_->Update();
	Vector3 position = player_->GetWorldPosition();

	modelFallRock_ = Model::CreateFromOBJ("soil");

	for (int i = 0; i < 200; i++) {
		fallRock_[i] = new FallRock;
		fallRock_[i]->Initialize(modelFallRock_, &viewProjection_);
		fallRock_[i]->SetMapChipField(mapChipField_);
	}

	viewProjection_.Initialize();
	viewProjection_.translation_ = {4, 3, -20.0f};
}

void Stage01::Update() {

	// 全ての当たり判定を行う
	CheckAllCollisions();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の作成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
			worldTransformBlock->UpdateMatrix();
		}
	}
	ImGui::Begin("camera");
	ImGui::DragFloat3("camera", &viewProjection_.translation_.x, 0.1f);
	ImGui::End();

	fallRock_[rockNum_]->Update();
	if (fallRock_[rockNum_]->GetMoveFinish() == true) {
		rockNum_++;
	}
	player_->Update();
	viewProjection_.UpdateMatrix();
}

void Stage01::Draw() {

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
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			blockModel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	for (int i = 0; i < 200; i++) {
		fallRock_[i]->Draw();
	}
	player_->Draw();

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

void Stage01::CheckAllCollisions() {
	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾すべての当たり判定
	for (FallRock* fallRock : fallRock_) {
		// 敵弾の座標
		aabb2 = fallRock->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び起こす
			player_->OnCollision(fallRock);
			// 敵弾の衝突時コールバックを呼び起こす
			fallRock->OnCollision(player_);
		}
	}
}
