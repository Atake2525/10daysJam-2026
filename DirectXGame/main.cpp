#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"
#include "Stage01.h"
#include "ResultScene.h"

enum class Scene {
	kUnknown = 0,

	kTitle,
	kStageSelect,
	kStage01,
	kClear,
	kDead,

};

// 現在シーン(型)
Scene scene = Scene::kUnknown;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
Stage01* stage01 = nullptr;
ClearScene* clearScene = nullptr;
DeadScene* deadScene = nullptr;
void ChangeScene();
void UpdateScene();
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"2062_マグマパニック");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	//// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		// シーンの切り替え
		ChangeScene();
		// 現在シーン更新
		UpdateScene();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		// 現在シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete gameScene;
	delete stage01;
	delete clearScene;
	delete deadScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() { 
switch (scene) {
	case Scene::kTitle:
	if (titleScene->IsFinished()) {
		// シーン変更
		scene = Scene::kStageSelect;
		// 旧シーンの解放
		delete titleScene;
		titleScene = nullptr;
		// 新シーンの作成と初期化
		gameScene = new GameScene;
		gameScene->Initialize();
	}
		break;
	case Scene::kStageSelect:
		if (gameScene->IsFinished()) {
			// シーン変更
			if (gameScene->SelectStage() == 1) {
				scene = Scene::kStage01;
				// 旧シーンの解放
				delete gameScene;
				gameScene = nullptr;
				// 新シーンの生成と初期化
				stage01 = new Stage01;
				stage01->Initialize();
			}
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinishedClear()) {
			// シーン変更
		    scene = Scene::kTitle;
			// 旧シーンの解放
			delete clearScene;
			clearScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kDead:
		if (deadScene->IsFinishedDead()) {
			// シーン変更
			scene = Scene::kDead;
			// 旧シーンの解放
			delete deadScene;
			deadScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	default:
		break;
	}
}


void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kStageSelect:
		gameScene->Update();
		break;
	case Scene::kStage01:
		stage01->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
		case Scene::kDead:
		deadScene->Update();
		break;
	default:
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kStageSelect:
		gameScene->Draw();
		break;
	case Scene::kStage01:
		stage01->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	case Scene::kDead:
		deadScene->Draw();
		break;
	default:
		break;
	}
}