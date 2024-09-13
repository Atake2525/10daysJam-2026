#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <AABB.h>
#include <Vector3.h>
#include <algorithm>
#include <numbers>
#include <Math.h>

class MapChipField;

class Goal;

class Player {
public:

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">プレイヤーモデル</param>
	/// <param name="position">プレイヤーの初期位置の設定</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// マップチップ当たり判定系
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }

	Vector3 GetWorldPosition();
	// AABBを取得
	AABB GetAABB();

	// ゲームクリア
	void isGameClear();

	// ゲームオーバー
	void isGameOver();

	// ゲームクリアフラグのgetter
	bool IsClear() const { return isClear_; }

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	private:
	   // マップチップによるフィールド
	    MapChipField* mapChipField_ = nullptr;
		WorldTransform worldTransform_;
	    ViewProjection* viewProjection_;

		Model* model_ = nullptr;

		// プレイヤーの速度(変更不可)
	    Vector3 velocity_ = {0};
		// プレイヤーの加速度(変更可)
	    float Acceleration_ = 0.005f;
		// ブレーキ強度(変更可)
	    static inline const float attenuation_ = 0.15f;
		// 速度の最大値(変更可)
	    static inline const float limitRunSpeed_ = 0.2f;
		
		// プレイヤーのジャンプ初速(変更可)
	    float JumpAcceleration_ = 0.4f;
	    // 重力加速度(下方向)変更可
	    static inline const float gravityAcceleration_ = 0.02f;
	    // 最大落下速度(下方向)変更可
	    static inline const float limitFallSpeed_ = 4.0f;

		// プレイヤーが地面についているかのフラグ(変更不可)
	    bool onGround = true;
	void OnCollision(const Goal* goal);

private:
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;

	Model* model_ = nullptr;

	// プレイヤーの速度(変更不可)
	Vector3 velocity_ = {0};
	// プレイヤーの加速度(変更可)
	float Acceleration_ = 0.15f;
	// ブレーキ強度(変更可)
	static inline const float attenuation_ = 0.1f;
	static inline const float kAttenuationWall = 0.2f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.15f;
	// プレイヤーのジャンプ初速(変更可)
	float JumpAcceleration_ = 20.0f;
	// 重力加速度(下方向)変更可
	static inline const float gravityAcceleration_ = 0.98f;
	// 最大落下速度(下方向)変更可
	static inline const float limitFallSpeed_ = 0.5f;
	// 速度の最大値(変更可)
	static inline const float limitRunSpeed_ = 0.1f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;
	static inline const float kGroundSearchHeight = 0.06f;

	// プレイヤーが地面についているかのフラグ(変更不可)
	bool onGround = true;
	//向き
	LRDirection lrDirection_ = LRDirection::kRight;

	struct CollisionMapInfo {
		bool ceiling = false; // 天井
		bool landing = false; // 着陸
		bool hitWall = false; // 壁
		Vector3 move;
	};
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// プレイヤー移動系処理
	void Move();

	void ChecMapCollision(CollisionMapInfo& info);
	void ChecMapCollisionUp(CollisionMapInfo& info);
	void ChecMapCollisionDown(CollisionMapInfo& info);
	void ChecMapCollisionRight(CollisionMapInfo& info);
	void ChecMapCollisionLeft(CollisionMapInfo& info);

	void CheckMapMove(const CollisionMapInfo& info);

	void CheckMapCelling(const CollisionMapInfo& info);

	void UpdateOnGround(const CollisionMapInfo& info);

	void CheckMapHItWall(const CollisionMapInfo& info);

	void AnimateTurn();

	Vector3 CornerPosition(const Vector3& center, Corner corner);


		// ゲームクリアフラグ
	    bool isClear_ = false;
	    // デスフラグ
	    bool isDead_ = false;

};
