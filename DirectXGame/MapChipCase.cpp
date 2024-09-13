#include "DebugText.h"
#include "MapChipCase.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include "imgui.h"

namespace {

std::map<std::string, MapChipCaseType> mapChipTable = {
    {"0", MapChipCaseType::kAir},
    {"1", MapChipCaseType::kRock},
};
}

void MapChipCase::ResetMapChipData() {
	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipCaseType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipCase::LoadMapChipCsv(const std::string& filePath) {
	// マップチップデータをリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	std::string line;

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して分析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
	mapChipLoaded_ = true;
}

MapChipCaseType MapChipCase::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipCaseType::kAir;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipCaseType::kAir;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipCase::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

MapChipCase::IndexSet MapChipCase::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = uint32_t((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>(position.y + kBlockHeight / 2.0f / kBlockHeight);
	return indexSet;
}

MapChipCase::Rect MapChipCase::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	// 指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect = {};
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}

void MapChipCase::Initialize() {
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			mapChipData_.data[i][j] = MapChipCaseType::kAir;
		}
	}
}

void MapChipCase::Update() {
	ImGui::Begin("MapChipCase");
	if (ImGui::Button("finish")) {
		isFinish_ = false;	
	}
	ImGui::End();
	if (isFinish_ == false) {
		mapCase = float(rand() % maxMapCase) + 1;
		isFinish_ = true;
		mapChipLoaded_ = false;
	}
}