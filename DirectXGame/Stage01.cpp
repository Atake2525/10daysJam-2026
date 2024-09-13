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
	worldTransformCaseBlocks_.clear();
	delete player_;
	delete mapChipField_;
	for (int i = 0; i < 200; i++) {
		delete fallRock_[i];
		delete rockBlock_[i];
	}
	delete searcher_;
}

void Stage01::Initialize() {
	// ランダムシードを初期化
	srand(static_cast<unsigned int>(time(NULL)));

	debug = Model::CreateFromOBJ("cube");

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
	mapChipCase_ = new MapChipCase;
	mapChipCase_->LoadMapChipCsv("Resources/case00.csv");
	GenerateCaseBlocks();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 18);
	player_ = new Player;
	modelPlayer_ = Model::CreateFromOBJ("player");
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	player_->Update();
	Vector3 position = player_->GetWorldPosition();

	modelFallRock_ = Model::CreateFromOBJ("soil");
	modelRockBlock_ = Model::CreateFromOBJ("glassFloor");

	searcher_ = new Searcher;
	searcher_->Initialize(debug, &viewProjection_);
	searcher_->SetMapChipCase(mapChipCase_);

	for (int i = 0; i < 200; i++) {
		fallRock_[i] = new FallRock;
		fallRock_[i]->Initialize(modelFallRock_, &viewProjection_);
		fallRock_[i]->SetMapChipField(mapChipField_);
		rockBlock_[i] = new RockBlock;
		rockBlock_[i]->Initialize(modelRockBlock_, &viewProjection_);
		rockBlock_[i]->SetMapChipField(mapChipField_);
		rockBlock_[i]->SetFallRock(fallRock_[i]);
		searcher_->SetRock(rockBlock_[i]);
	}

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
		mapChipCase_->Initialize();
		if (mapChipCase_->mapChipLoaded_ == false) {
			worldTransformCaseBlocks_.clear();
			mapChipCase_->LoadMapChipCsv("Resources/case01.csv");
			GenerateCaseBlocks();
		}
	} else if (mapChipCase_->GetMapCase() == 2) {
		mapChipCase_->Initialize();
		if (mapChipCase_->mapChipLoaded_ == false) {
			worldTransformCaseBlocks_.clear();
			mapChipCase_->LoadMapChipCsv("Resources/case02.csv");
			GenerateCaseBlocks();
		}
	} else if (mapChipCase_->GetMapCase() == 3) {
		mapChipCase_->Initialize();
		if (mapChipCase_->mapChipLoaded_ == false) {
			worldTransformCaseBlocks_.clear();
			mapChipCase_->LoadMapChipCsv("Resources/case03.csv");
			GenerateCaseBlocks();
		}
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

	// 形の検索
	// 要素数
	const uint32_t kNumBlockVirtical = mapChipCase_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipCase_->GetNumBlockHorizontal();

	//Caseの計算
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			// for文でi,jの位置にブロックがあるか検索
			if (mapChipCase_->GetMapChipTypeByIndex(j, i) == MapChipCaseType::kRock) {
				searchCaseNums_[3 - i][j] = 1;
				searchCaseNum_ += 1;
			}
			//// マップチップのブロックとfallBlockが同じ位置にあるか検索
			// if (searchBlockNums_[j][i] == 1 && mapChipCase_->GetMapChipTypeByIndex(j, i + 4) == MapChipCaseType::kRock) {
			//	searchBlocks_ += 1;
			// }
		}
	}
	// rockBlock計算
	for (int k = 0; k < 200; k++) {
		// searcher_->SetRock(rockBlock_[k]);
		// searcher_->CaseCollision(Vector3{numWidthSearch_, numHeightSearch_, 0});
		Vector3 fallRockPos = rockBlock_[k]->GetWrodlTransform();

		for (uint32_t i = 0 + uint32_t(numHeightSearch_); i < 4; i++) {
			for (uint32_t j = 0; j < 4; j++) {
				if ((fallRockPos.x == searcher_->GetBlockWorldTransform().x + j + numWidthSearch_ && fallRockPos.y == searcher_->GetBlockWorldTransform().y - i + 1 + numHeightSearch_) &&
				    rockBlock_[k]->GetisClear() == false) {
					searchBlockNums_[uint32_t(fallRockPos.y - 1 + numHeightSearch_)][uint32_t(fallRockPos.x)] = 1;
				}
			}
		}
		// 以上の検索結果の数が一致しているか
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (searchBlockNums_[i][j] == 1 && searchCaseNums_[i][j] == 1 && searchBlockNums_[i][j] == searchCaseNums_[i][j]) {
					searchBlocks_ += 1;
					if (searchBlockNums_[i][j + 1] == 1) {
						maxSearchPosition_ += 1 * 1.0f;
					}
				}
			}
		}

	if (searchBlocks_ == searchCaseNum_) {
		mapChipCase_->isFinish_ = false;
	}
	else {
		searchBlocks_ = 0;
	}
		if (searcher_->GetIsEqual()) {
			mapChipCase_->isFinish_ = false;
			searcher_->SetIsEqual(false);
			ResetSearch();
			numHeightSearch_ = maxSearchPosition_;
			if (numWidthSearch_ + 4 > rockBlock_[k]->GetWrodlTransform().x && numHeightSearch_ + 4 > rockBlock_[k]->GetWrodlTransform().y) {
				rockBlock_[k]->SetClear(true);
			}
		}
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
	searcher_->Draw();

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

void Stage01::ResetSearch() {
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			searchBlockNums_[i][j] = 0;
			searchCaseNums_[i][j] = 0;
			searchBlocks_ = 0;
			searchCaseNum_ = 0;
		}
	}
}
