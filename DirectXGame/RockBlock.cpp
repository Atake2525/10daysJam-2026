#include "RockBlock.h"
#include "MapChipField.h"
#include "FallRock.h"
#include "imgui.h"
#include "kMath.h"

void RockBlock::Initialize(Model* model, ViewProjection* viewProjection) {
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_= {-10.0f, -10.0f, -10.0f};
}

Vector3 RockBlock::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kBlockWidth / 2.0f, -kBlockHeight / 2.0f, 0}, // kRightBottom
	    {-kBlockWidth / 2.0f, -kBlockHeight / 2.0f, 0}, // kLeftBottom
	    {+kBlockWidth / 2.0f, +kBlockHeight / 2.0f, 0}, // kRightTop
	    {-kBlockWidth / 2.0f, +kBlockHeight / 2.0f, 0}  // kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void RockBlock::CollisionRock() {
	//for (int i = 0; i < 200; i++) {
		//if (worldTransform_.translation_.x == fallRock_[i]->GetWrodlTransform().x && worldTransform_.translation_.y + 1 == fallRock_[i]->GetWrodlTransform().y) {
			//fallRock_[i]->SetMoveFinish(true);
		//}
	//}
}

void RockBlock::Update() {

			if (fallRock_->GetMoveFinish()) {
				    worldTransform_.translation_ = fallRock_->GetWrodlTransform();
			}
	
	worldTransform_.UpdateMatrix();
	ImGui::Begin("RockBlock config");
	ImGui::DragFloat3("worldTransform", &worldTransform_.translation_.x, 1.0f);
	ImGui::End();
}

void RockBlock::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
