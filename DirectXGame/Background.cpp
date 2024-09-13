#include "Background.h"
#include "cassert"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void Skydome::Update() { 
	worldTransform_.TransferMatrix(); 
	worldTransform_.UpdateMatrix();
}
void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Magma::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void Magma::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}
void Magma::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void StonePillar::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void StonePillar::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}
void StonePillar::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
