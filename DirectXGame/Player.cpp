#define NOMINMAX
#include "Player.h"
#include "kMath.h"
#include <algorithm>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position) { 
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

// プレイヤー移動系
void Player::Move() {
	if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_A)) {
			if (velocity_.x > 0) {
				velocity_.x *= (1.0f - attenuation_);
			}
			acceleration.x -= Acceleration_;
		}
		if (Input::GetInstance()->PushKey(DIK_D)) {
			if (velocity_.x < 0) {
				velocity_.x *= (1.0f - attenuation_);
			}
			acceleration.x += Acceleration_;
		}
		velocity_.x += acceleration.x;
		// 速度制御
		velocity_.x = std::clamp(velocity_.x, -limitRunSpeed_, limitRunSpeed_);
	} else {
		// 何もキーを入力していない場合減速する
		velocity_.x *= (1.0f - attenuation_);
	}
	if (onGround) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			Vector3 jumpAcceleration = {0, JumpAcceleration_, 0};
			velocity_ += jumpAcceleration;
		}
		if (velocity_.y > 0) {
			onGround = false;
		}
	} else {
		// ジャンプからの落下
		velocity_ += Vector3(0, -gravityAcceleration_, 0);
		velocity_.y = std::max(velocity_.y, -limitFallSpeed_);
	}
	worldTransform_.translation_ += velocity_;
}

void Player::Update() { 
	Move(); 
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }