#pragma once
#include "Model.h"
#include "WorldTransform.h"

enum class MapChipCaseType {
	kAir, // 空白
	kRock, // ブロック
};

struct MapChipCaseData {
	std::vector<std::vector<MapChipCaseType>> data;
};

// std::unordered_map<std::string, MapChipType> mapChipTable = {
//     {"0", MapChipType::kBlank},
//     {"1", MapChipType::KBlock},
// };

class MapChipCase {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};
	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};

public:
	// 1ブロックのサイズ
	// 幅
	static inline const float kBlockWidth = 1.0f;
	// 高さ
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	// 横
	static inline const uint32_t kNumBlockHorizontal = 4;
	// 縦
	static inline const uint32_t kNumBlockVirtical = 4;

	const uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }

	const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	MapChipCaseData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipCaseType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	WorldTransform worldTransform_;

	bool isFinish_ = false;

	float mapCase = 0.0f;
	// 見本の数(変更可)
	static inline const uint32_t maxMapCase = 3;

	float GetMapCase() { return mapCase; }

	bool mapChipLoaded_ = false;

	void Update();

	void Initialize();

};
