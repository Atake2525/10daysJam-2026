#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MapChipField.h"

/// <summary>
/// ゲームシーン
/// </summary>
class Stage01 {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Stage01();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage01();

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
	bool IsFinihed() const { return finished_; }

	// ステージgetter
	int GetStage() const { return 1; }


	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	bool finished_ = false;

	MapChipField* mapChipField_ = nullptr;
	Model* blockModel_ = nullptr;

	ViewProjection viewProjection_;

	void GenerateBlocks();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
