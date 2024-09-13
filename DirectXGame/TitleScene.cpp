#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() { 
	delete titleFont_;
	delete modelTitle_;
	delete spaceKeyFont_;
	delete modelSpaceKey_;
	delete skydome_;
	delete modelSkydome_;
	delete magma_;
	delete modelMagma_;
	delete stonePillar1_;
	delete modelStonePillar1_;
	delete stonePillar2_;
	delete modelStonePillar2_;

}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	Vector3 titlePosition = {-20, 10, 0};
	titleFont_ = new TitleFont();
	modelTitle_ = Model::CreateFromOBJ("titleFont");
	titleFont_->Initialize(modelTitle_, &viewProjection_, titlePosition);
	Vector3 SpaceKeyPosition = {-15, -1, 0};
	spaceKeyFont_ = new SpaceKeyFont();
	modelSpaceKey_ = Model::CreateFromOBJ("spaceKeyFont");
	spaceKeyFont_->Initialize(modelSpaceKey_, &viewProjection_, SpaceKeyPosition);

	Vector3 skydomePosition = {0, 0, 0};
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skyDome");
	skydome_->Initialize(modelSkydome_, &viewProjection_, skydomePosition);
	Vector3 magmaPosition = {0, -15, -30};
	magma_ = new Magma();
	modelMagma_ = Model::CreateFromOBJ("magma");
	magma_->Initialize(modelMagma_, &viewProjection_, magmaPosition);
	Vector3 stonePillerPosition1 = {17, 0, -25};
	stonePillar1_ = new StonePillar();
	modelStonePillar1_ = Model::CreateFromOBJ("stonePillar");
	stonePillar1_->Initialize(modelStonePillar1_, &viewProjection_, stonePillerPosition1);
	Vector3 stonePillerPosition2 = {-17, 0, -25};
	stonePillar2_ = new StonePillar();
	modelStonePillar2_ = Model::CreateFromOBJ("stonePillar");
	stonePillar2_->Initialize(modelStonePillar2_, &viewProjection_, stonePillerPosition2);


	finished_ = false;
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
	titleFont_->Update();
	spaceKeyFont_->Update();
	skydome_->Update();
	magma_->Update();
	stonePillar1_->Update();
	stonePillar2_->Update();

}

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	titleFont_->Draw();
	spaceKeyFont_->Draw();
	skydome_->Draw();
	magma_->Draw();
	stonePillar1_->Draw();
	stonePillar2_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
