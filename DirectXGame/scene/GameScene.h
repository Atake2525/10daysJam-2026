#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Stage01.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	// ステージセレクトのgetter
	int SelectStage() { return selectStage_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	bool finished_ = false;
	// 選択されているステージ
	int selectStage_ = 1;
	// ステージの数
	int StageNum = 1;

	Stage01* stage01 = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
