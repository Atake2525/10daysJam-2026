#include"Font.h"
#include "cassert"

void TitleFont::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void TitleFont::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}
void TitleFont::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void SpaceKeyFont::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void SpaceKeyFont::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}
void SpaceKeyFont::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void ClearFont::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
  worldTransform_.translation_ = position;
 }
void ClearFont::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}
void ClearFont::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void DeadFont::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
  worldTransform_.translation_ = position;
 }
void DeadFont::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}
void DeadFont::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
