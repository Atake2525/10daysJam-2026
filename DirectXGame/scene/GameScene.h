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
	bool IsFinished() const { return finished_; }

	// ステージセレクトのgetter
	int SelectStage() { return selectStage_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Model* Stage01Model_ = nullptr;

	WorldTransform cubeWorldTransform_;
	Model* SelectCubeModel_ = nullptr;

	bool finished_ = false;
	// 選択されているステージ(1固定)
	int selectStage_ = 1;

	Stage01* stage01 = nullptr;

	// 現在のステージの場所取得
	Vector3 nowPosition = {0};

	//----------
	// ここから下は変更可能

	/// <summary>
	/// ターンアニメーション
	/// </summary>
	// タイマー
	float turnTimer_ = 0.0f;
	// アニメーションの時間
	float turntime_ = 2.0f;
	// 左旋回
	bool LeftTurnFlag = false;
	// 右旋回
	bool RightTurnFlag = false;

	// ステージの数(ステージ作る度に増やす)
	const int StageNum = 2;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
