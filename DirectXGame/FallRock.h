#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <AABB.h>
#include <Vector3.h>
#include <algorithm>
#include <numbers>

class MapChipField;
class Player;
class FallRock {
public:
	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};
	struct CollisionMapInfo {
		// 地面確認フラグ
		bool isLand = false;
		// 移動量
		Vector3 movePoint;
	};
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 移動関係
	void Move();

	// 衝突判定
	void Collision(CollisionMapInfo& info);

	// 衝突判定プレイヤーと
	void OnCollision(const Player* player);
	Vector3 GetworldPosition();
	AABB GetAABB();

	// setter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// getter
	int32_t GetMaxBlockNum() { return maxBlockNum_; }

	bool GetIsMove() { return isMove_; }
	bool GetMoveFinish() { return moveFinish_; }

	Vector3 GetWrodlTransform() { return worldTransform_.translation_; }

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	Model* model_ = nullptr;

	// ブロック数の上限
	int32_t maxBlockNum_ = 200;

	// 1ブロックのサイズ
	// 幅
	static inline const float kBlockWidth = 1.0f;
	// 高さ
	static inline const float kBlockHeight = 1.0f;

	// ブロック落下のタイマー(変更不可)
	float fallTimer_ = 0.0f;
	// ブロックが落下するまでのタイム(変更可)
	float fallLimitTime_ = 1.0f;
	// 動かしたかのフラグ
	bool moveFinish_ = false;
	// 動いているかのフラグ
	bool isMove_ = false;
};
