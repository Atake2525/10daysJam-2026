#include "FallRock.h"
#include "MapChipField.h"
#include "imgui.h"
#include "kMath.h"

void FallRock::Initialize(Model* model, ViewProjection* viewProjection) { 
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {-10.0f, -10.0f, -10.0f};
	// ランダムシードを初期化
	srand(static_cast<unsigned int>(time(NULL)));
	moveFinish_ = false;
}

Vector3 FallRock::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kBlockWidth / 2.0f, -kBlockHeight / 2.0f, 0}, // kRightBottom
	    {-kBlockWidth / 2.0f, -kBlockHeight / 2.0f, 0}, // kLeftBottom
	    {+kBlockWidth / 2.0f, +kBlockHeight / 2.0f, 0}, // kRightTop
	    {-kBlockWidth / 2.0f, +kBlockHeight / 2.0f, 0}  // kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void FallRock::Move() {
	// fallLimitTime_秒ごとに１マスずつ落下する
	if (isMove_) {
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fallTimer_ += 0.1f;
		}
		// 右を入力で移動
		if (Input::GetInstance()->TriggerKey(DIK_RIGHT) && mapChipField_->GetNumBlockHorizontal() - 1 > worldTransform_.translation_.x) {
			worldTransform_.translation_.x += 1 * kBlockWidth;
		}
		if (Input::GetInstance()->TriggerKey(DIK_LEFT) && 0 < worldTransform_.translation_.x) {
			worldTransform_.translation_.x -= 1 * kBlockWidth;
		}
	}
	fallTimer_ += 1 / fallLimitTime_ / 60;
	if (fallTimer_ > 1) {
		worldTransform_.translation_.y -= mapChipField_->kBlockHeight;
		fallTimer_ = 0.0f;
		if (isMove_ == false) {
			worldTransform_.translation_.z = 0;
			worldTransform_.translation_.x = float(rand() % mapChipField_->GetNumBlockHorizontal());
			worldTransform_.translation_.y = 10;
			isMove_ = true;
		}
	}
}

void FallRock::Collision(CollisionMapInfo& info) {
	// 移動後の4つ角の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.movePoint, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 左の当たり判定を行う
	MapChipField::IndexSet indexSet;
	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kStone) {
		info.isLand = true;
		//isMove_ = false;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kStone) {
		info.isLand = true;
		//isMove_ = false;
		//mapChipField_->mapChipData_.data[indexSet.yIndex + 1][indexSet.xIndex] = MapChipType::kStone;
	}
}

void FallRock::Update() { 
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.movePoint = {0, -1, 0};
	Move();
	Collision(collisionMapInfo);

	if (collisionMapInfo.isLand) {
		isMove_ = false;
		moveFinish_ = true;
		mapChipField_->mapChipData_.data[uint32_t(worldTransform_.translation_.y)][uint32_t(worldTransform_.translation_.x)] = MapChipType::kStone;
	}
	worldTransform_.UpdateMatrix();
	ImGui::Begin("FallRock config");
	ImGui::SliderFloat("FallTimer", &fallTimer_, 0.0f, 1.0f);
	ImGui::DragFloat3("worldTransform", &worldTransform_.translation_.x, 1.0f);
	if (ImGui::Button("isMove")) {
		isMove_ = false;
		moveFinish_ = true;
	}
	ImGui::End();
}

void FallRock::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

