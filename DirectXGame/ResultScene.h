#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"

class ClearScene {
	
public:
	ClearScene();
	~ClearScene();
	void Initialize();
	void Update();
	void Draw();

	// 終了フラグのgetter
	bool IsFinishedClear() const { return finishedClear_; }

private:
	Model* modelClearFont_ = nullptr;
	ViewProjection viewProjection_;
	WorldTransform worldTransformFont_;
	WorldTransform worldTransformPlayer_;
	DirectXCommon* dxCommon_ = nullptr;

	// 終了フラグ
	bool finishedClear_ = false;

	
};

class DeadScene {

public:
	DeadScene();
	~DeadScene();
	void Initialize();
	void Update();
	void Draw();

	// 終了フラグのgetter
	bool IsFinishedDead() const { return finishedDead_; }

private:
	Model* modelDeadFont_ = nullptr;
	ViewProjection viewProjection_;
	WorldTransform worldTransformFont_;
	WorldTransform worldTransformPlayer_;
	DirectXCommon* dxCommon_ = nullptr;

	// 終了フラグ
	bool finishedDead_ = false;
};