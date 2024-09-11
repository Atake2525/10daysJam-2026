#include"ResultScene.h"
#include <numbers>


ClearScene::ClearScene() {}
ClearScene::~ClearScene() { 
	delete modelClearFont_; 
}
void ClearScene::Initialize() {
	modelClearFont_ = Model::CreateFromOBJ("clearFont");
	viewProjection_.Initialize();
	worldTransformFont_.Initialize();
	worldTransformPlayer_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();

}
void ClearScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finishedClear_ = true;
	}

}
void ClearScene::Draw() {}

DeadScene::DeadScene() {}
DeadScene::~DeadScene() { delete modelDeadFont_; }
void DeadScene::Initialize() {
	modelDeadFont_ = Model::CreateFromOBJ("deadFont");
	viewProjection_.Initialize();
	worldTransformFont_.Initialize();
	worldTransformPlayer_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
}
void DeadScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finishedDead_ = true;
	}

}
void DeadScene::Draw() {}
