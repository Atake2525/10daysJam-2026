#include "AABB.h"
#include "FallRock.h"
#include "MapChipCase.h"
#include "Searcher.h"
#include "kMath.h"
#include "RockBlock.h"
#include "imgui.h"

void Searcher::Initialize(Model* model, ViewProjection* viewProjection) {
	model_ = model;
	viewProjection_ = viewProjection;
	blockWorldTransform_.Initialize();
	caseWorldTransform_.Initialize();
	caseWorldTransform_.translation_ = {14, 6, 0};
}

Vector3 Searcher::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kBlockWidth / 2.0f, -kBlockHeight / 2.0f, 0}, // kRightBottom
	    {-kBlockWidth / 2.0f, -kBlockHeight / 2.0f, 0}, // kLeftBottom
	    {+kBlockWidth / 2.0f, +kBlockHeight / 2.0f, 0}, // kRightTop
	    {-kBlockWidth / 2.0f, +kBlockHeight / 2.0f, 0}  // kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Searcher::CaseCollision(Vector3 searchPosition) {
	/*// 移動後の4つ角の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(caseWorldTransform_.translation_ + info.movePoint, static_cast<Corner>(i));
	}

	MapChipCaseType mapChipType;
	// 当たり判定を行う
	MapChipCase::IndexSet indexSet;
	// 左上点の判定*/
			/* indexSet = mapChipCase_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
			mapChipType = mapChipCase_->GetMapChipTypeByIndex(indexSet.xIndex + j, indexSet.yIndex + uint32_t(searchHeight) - i);
			if (mapChipType == MapChipCaseType::kRock) {
			    searchCase_ = 1;
			    //searchCaseNum_ += 1;
			    // isMove_ = false;
			}
			if (mapChipType == MapChipCaseType::kAir) {
			    searchCase_ = 0;
			    // isMove_ = false;
			}*/

		Vector3 fallRockPos = fallRock_->GetWrodlTransform();
	if ((fallRockPos.x == blockWorldTransform_.translation_.x + searchPosition.x && fallRockPos.y == blockWorldTransform_.translation_.y + searchPosition.y) &&
	    fallRock_->GetisClear() == false) {
		searchBlockNums_[uint32_t(fallRockPos.x)][uint32_t(fallRockPos.y - 1)] = 1;
	}
			// 要素数
			const uint32_t kNumBlockVirtical = mapChipCase_->GetNumBlockVirtical();
			const uint32_t kNumBlockHorizontal = mapChipCase_->GetNumBlockHorizontal();

			for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
				for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
					// for文でi,jの位置にブロックがあるか検索
					if (mapChipCase_->GetMapChipTypeByIndex(j, i + 4) == MapChipCaseType::kRock) {
						searchCaseNum_ += 1;
					}
					// マップチップのブロックとfallBlockが同じ位置にあるか検索
					if (searchBlockNums_[j][i] == 1 && mapChipCase_->GetMapChipTypeByIndex(j, i) == MapChipCaseType::kRock) {
						searchBlocks_+= 1;
					}
				}
			}
	        for (uint32_t i = 0; i < 200; i++) {
		        for (uint32_t j = 0; j < 200; j++) {
			        if (mapChipCase_->GetMapChipTypeByIndex(j, i) == MapChipCaseType::kRock) {
				        searchCaseNum_++;
			        }
		        }
	        }
			// 以上の検索結果の数が一致しているか
	        if (searchCaseNum_ == searchBlocks_ && searchCaseNum_ > 0) {
				isEqual_ = true;
	        } else {
		        searchCaseNum_ = 0;
		        searchBlocks_ = 0;
	        }

			ImGui::Begin("Search");
	        ImGui::DragFloat3("translation", &blockWorldTransform_.translation_.x, 1.0f);
	        ImGui::End();
	blockWorldTransform_.UpdateMatrix();
	caseWorldTransform_.UpdateMatrix();
}

void Searcher::ResetSearchBlocks() {
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			searchBlockNums_[j][i] = 0;
		}
	}
}

void Searcher::Update() {
	/*// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.movePoint = {0, 0, 0};*/

	//CaseCollision(/* collisionMapInfo*/);
}

void Searcher::Draw() {
	model_->Draw(blockWorldTransform_, *viewProjection_);
	model_->Draw(caseWorldTransform_, *viewProjection_);
}
