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

void Stage01::GenerateCaseBlocks() {
	// 要素数
	const uint32_t kNumBlockVirtical = mapChipCase_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipCase_->GetNumBlockHorizontal();

	// 列数を設定(縦方向ブロック数)
	worldTransformCaseBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		// 1列の要素数を設定(横方向ブロック数)
		worldTransformCaseBlocks_[i].resize(kNumBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipCase_->GetMapChipTypeByIndex(j, i) == MapChipCaseType::kRock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformCaseBlocks_[i][j] = worldTransform;
				worldTransformCaseBlocks_[i][j]->translation_ = mapChipCase_->GetMapChipPositionByIndex(j + 14, i - 6);
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
	for (std::vector<WorldTransform*>& worldTransformCaseBlockLine : worldTransformCaseBlocks_) {
		for (WorldTransform* worldTransformCaseBlock : worldTransformCaseBlockLine) {
			delete worldTransformCaseBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete player_;
	delete mapChipField_;
	for (int i = 0; i < 200; i++) {
		delete fallRock_[i];
		delete rockBlock_[i];
	}

	delete skydome_;
	delete modelSkydome_;

}

void Stage01::Initialize() {
	// ランダムシードを初期化
	srand(static_cast<unsigned int>(time(NULL)));

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/Stage01.csv");
	blockModel_ = Model::CreateFromOBJ("block1");

	GenerateBlocks();
	mapChipCase_ = new MapChipCase;
	mapChipCase_->LoadMapChipCsv("Resources/case00.csv");
	GenerateCaseBlocks();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 19);
	player_ = new Player;
	modelPlayer_ = Model::CreateFromOBJ("player");
	player_->Initialize(modelPlayer_, &viewProjection_, {0, 0, 0});
	player_->SetMapChipField(mapChipField_);

	player_->Update();
	Vector3 position = player_->GetWorldPosition();

	modelFallRock_ = Model::CreateFromOBJ("block2");
	modelRockBlock_ = Model::CreateFromOBJ("block2");


	for (int i = 0; i < 200; i++) {
		fallRock_[i] = new FallRock;
		fallRock_[i]->Initialize(modelFallRock_, &viewProjection_);
		fallRock_[i]->SetMapChipField(mapChipField_);
		rockBlock_[i] = new RockBlock;
		rockBlock_[i]->Initialize(modelRockBlock_, &viewProjection_);
		rockBlock_[i]->SetMapChipField(mapChipField_);
		rockBlock_[i]->SetFallRock(fallRock_[i]);
	}

	Vector3 skydomePosition = {0, 0, 0};
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skyDome");
	skydome_->Initialize(modelSkydome_, &viewProjection_, skydomePosition);



	viewProjection_.Initialize();
	viewProjection_.translation_ = {4, 3, -20.0f};
	//viewProjection_.translation_ = {4.5f, 10.0f, -15.0f};
	//viewProjection_.rotation_ = {0.3f, 0.0f, 0.0f};

}

void Stage01::Update() {
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
	mapChipCase_->Update();
	if (mapChipCase_->GetMapCase() == 1) {
		mapChipCase_->LoadMapChipCsv("Resources/case01.csv");
		GenerateCaseBlocks();
	} else if (mapChipCase_->GetMapCase() == 2) {
		mapChipCase_->LoadMapChipCsv("Resources/case02.csv");
		GenerateCaseBlocks();
	} else if (mapChipCase_->GetMapCase() == 3) {
		mapChipCase_->LoadMapChipCsv("Resources/case03.csv");
		GenerateCaseBlocks();
	}
	for (std::vector<WorldTransform*>& worldTransformCaseBlockLine : worldTransformCaseBlocks_) {
		for (WorldTransform* worldTransformCaseBlock : worldTransformCaseBlockLine) {
			if (!worldTransformCaseBlock)
				continue;
				// アフィン変換行列の作成
				worldTransformCaseBlock->matWorld_ = MakeAffineMatrix(worldTransformCaseBlock->scale_, worldTransformCaseBlock->rotation_, worldTransformCaseBlock->translation_);
				// 定数バッファに転送する
			    //worldTransformCaseBlock->translation_ = {10.0f, 10.0f, 0.0f};
				worldTransformCaseBlock->TransferMatrix();
				worldTransformCaseBlock->UpdateMatrix();
		}
	}
	ImGui::Begin("camera");
	ImGui::DragFloat3("Translation", &viewProjection_.translation_.x, 0.1f);
	ImGui::DragFloat3("Rotation", &viewProjection_.rotation_.x, 0.1f);
	ImGui::End();

	player_->Update();
	fallRock_[rockNum_]->Update();
	rockBlock_[rockNum_]->Update();
	if (fallRock_[rockNum_]->GetMoveFinish() == true) {
		rockNum_++;
	}
	// ブロックの重なりの解消
	// ブロックが積み重なるようにする
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			if (rockBlock_[i]->GetWrodlTransform().x == fallRock_[j]->GetWrodlTransform().x && rockBlock_[i]->GetWrodlTransform().y + 1 == fallRock_[j]->GetWrodlTransform().y) {
				fallRock_[j]->SetMoveFinish(true);
			}
			// 思った動作をしないためコメントアウト
			/*if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
				if (rockBlock_[i]->GetWrodlTransform().y == fallRock_[j]->GetWrodlTransform().y && rockBlock_[i]->GetWrodlTransform().x + 1 == fallRock_[j]->GetWrodlTransform().x) {
					fallRock_[j]->SetMoveFinish(true);
				}
			} else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
				if (rockBlock_[i]->GetWrodlTransform().y == fallRock_[j]->GetWrodlTransform().y && rockBlock_[i]->GetWrodlTransform().x - 1 == fallRock_[j]->GetWrodlTransform().x) {
					fallRock_[j]->SetMoveFinish(true);
				}
			}*/
		}
	}

	skydome_->Update();

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
	for (std::vector<WorldTransform*>& worldtransformCaseBlockLine : worldTransformCaseBlocks_) {
		for (WorldTransform* worldTransformCaseBlock : worldtransformCaseBlockLine) {
			if (!worldTransformCaseBlock)
				continue;
			blockModel_->Draw(*worldTransformCaseBlock, viewProjection_);
		}
	}

	for (int i = 0; i < 200; i++) {
		fallRock_[i]->Draw();
		//rockBlock_[i]->Draw();
	}
	player_->Draw();

	skydome_->Draw();


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
