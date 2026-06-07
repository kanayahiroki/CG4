#include "GameScene.h"
#include <numbers>

GameScene::~GameScene() { Effect::StaticFinalize(); }

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("uvChecker.png");

	Effect::StaticInitialize();

	// モデル生成（まずは簡単に四角）
	model_ = Effect::CreateEffect();

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	worldTransform_.rotation_.z=std::numbers::pi_v<float>/ 2.0f;

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};

	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() {
	// ★これ追加（超重要）
	upData_->WorldTransformUpData(worldTransform_);

	camera_.UpdateMatrix();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// Model描画開始
	Effect::PreDraw(commandList);

	// ★ここで描画
	model_->Draw(worldTransform_, camera_, textureHandle_);

	// Model描画終了
	Effect::PostDraw();
}