#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MapChipField.h"
#include "Player.h"
#include "FallRock.h"
#include "RockBlock.h"
#include "MapChipCase.h"
#include "Goal.h"

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

	void CheckAllCollisions();

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	std::vector<std::vector<WorldTransform*>> worldTransformCaseBlocks_;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	Model* modelPlayer_;

	bool finished_ = false;

	MapChipField* mapChipField_ = nullptr;
	Model* blockModel_ = nullptr;

	MapChipCase* mapChipCase_ = nullptr;

	FallRock* fallRock_[200];
	Model* modelFallRock_ = nullptr;

	RockBlock* rockBlock_[200];
	Model* modelRockBlock_ = nullptr;

	int rockNum_ = 0;

	Goal* goal_ = nullptr;
	Model* modelGoal_ = nullptr;
	std::list<Goal*> goalies_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;


	void GenerateBlocks();
	void GenerateCaseBlocks();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
