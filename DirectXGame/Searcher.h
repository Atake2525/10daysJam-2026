#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 見本とブロックが一致しているかを検索するクラス(変更非推奨)
/// </summary>

class MapChipCase;
class RockBlock;

class Searcher {
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

	// 当たり判定
	void CaseCollision(Vector3 searchPosition);
	// void RockCollision(CollisionMapInfo& info);
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// getter
	float GetSearchWidth() { return searchWidth; }
	float GetSearchHeight() { return searchHeight; }
	float GetIsEqual() { return isEqual_; }
	Vector3 GetBlockWorldTransform() { return blockWorldTransform_.translation_; }

	// setter
	void SetMapChipCase(MapChipCase* mapChipCase) { mapChipCase_ = mapChipCase; }
	void SetRock(RockBlock* fallRock) { fallRock_ = fallRock; }
	void ResetSearchBlocks();
	void SetIsEqual(bool isEqual) { isEqual_ = isEqual; }

private:
	// 基本の初期化
	WorldTransform blockWorldTransform_;
	WorldTransform caseWorldTransform_;
	ViewProjection* viewProjection_;
	Model* model_ = nullptr;

	MapChipCase* mapChipCase_ = nullptr;
	RockBlock* fallRock_ = nullptr;

	// 見本ブロックの数を検索(変更不可)
	float searchCase_ = 0.0f;
	// 見本ブロックの数(変更不可)
	int searchCaseNum_ = 0;
	// 岩ブロックの数を検索(変更不可)
	int searchBlocks_ = 0;
	// 検索ブロックの数確認(変更不可)
	float searchNum_ = 0.0f;
	// 照合成功かのフラグ(変更不可)
	bool isEqual_ = false;

	// サーチ範囲
	// 幅(変更可)
	float searchWidth = 4.0f;
	// 高さ(変更可)
	float searchHeight = 4.0f;

	float searchBlockNums_[4][4] = {0};

	// 1ブロックのサイズ
	// 幅(変更不可)
	static inline const float kBlockWidth = 1.0f;
	// 高さ(変更不可)
	static inline const float kBlockHeight = 1.0f;
};
