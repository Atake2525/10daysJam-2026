#include "Goal.h"
#include "cassert"
#include <numbers>

void Goal::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

}

void Goal::Update() {
	// 　行列を更新
	worldTransform_.UpdateMatrix();

}

void Goal::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);

}

Vector3 Goal::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Goal::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	aabb.max = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Goal::OnCollision(const Player* player) { 
	(void)player;
}
