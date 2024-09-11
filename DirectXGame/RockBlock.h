#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField;
class FallRock;

class RockBlock {
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

	// FallRockとの当たり判定
	void CollisionRock();

	// setter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void SetFallRock(FallRock* fallRock) { fallRock_ = fallRock; }

	Vector3 GetWrodlTransform() { return worldTransform_.translation_; }

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	MapChipField* mapChipField_ = nullptr;
	FallRock* fallRock_;

	Model* model_ = nullptr;

	// 1ブロックのサイズ
	// 幅
	static inline const float kBlockWidth = 1.0f;
	// 高さ
	static inline const float kBlockHeight = 1.0f;

	 // ブロック落下のタイマー(変更不可)
	float fallTimer_ = 0.0f;
	// ブロックが落下するまでのタイム(変更可)
	float fallLimitTime_ = 1.0f;

};
