#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Font.h"
#include "Background.h"

/// <summary>
/// ゲームシーン
/// </summary>
class TitleScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// シーン切り替えのgetter
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	
	TitleFont* titleFont_ = nullptr;
	Model* modelTitle_ = nullptr;
	SpaceKeyFont* spaceKeyFont_ = nullptr;
	Model* modelSpaceKey_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Magma* magma_ = nullptr;
	Model* modelMagma_ = nullptr;
	StonePillar* stonePillar1_ = nullptr;
	Model* modelStonePillar1_ = nullptr;
	StonePillar* stonePillar2_ = nullptr;
	Model* modelStonePillar2_ = nullptr;

	bool finished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
