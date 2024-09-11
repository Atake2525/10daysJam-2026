#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"

class MapChipField;

class Player {
public:
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

	// プレイヤー移動系処理
	void Move();

	// マップチップ当たり判定系
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

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

		// ゲームクリアフラグ
	    bool isClear_ = false;
	    // デスフラグ
	    bool isDead_ = false;

};
