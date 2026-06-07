#include "GameScene.h"
#include <cstdlib>
#include <ctime>
#include <numbers>

GameScene::~GameScene() { Effect::StaticFinalize(); }

void GameScene::Initialize() {

	// 乱数の初期化
	srand((unsigned int)time(nullptr));

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	Effect::StaticInitialize();

	// モデル生成（まずは簡単に四角）
	model_ = Effect::CreateEffect();

	for (int i = 0; i < maxEffect; i++) {

		// ワールドトランスフォーム初期化
		worldTransforms_[i].Initialize();

		// 0～360の乱数を発生させる
		float degres = (float)(rand() % 360);

		// ラジアンに変換
		float radius = degres * (std::numbers::pi_v<float> / 180.0f);

		// 回転角をセット
		worldTransforms_[i].rotation_.z = radius;

		// 位置をセット
		worldTransforms_[i].translation_.z = i * 0.01f;
	}

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};

	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() {
	for (int i = 0; i < maxEffect; i++) {

		upData_->WorldTransformUpData(worldTransforms_[i]);
	}

	camera_.UpdateMatrix();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// Model描画開始
	Effect::PreDraw(commandList);

	for(int i = 0; i < maxEffect; i++) 
	{
		model_->Draw(worldTransforms_[i], camera_, textureHandle_);
	}

	// Model描画終了
	Effect::PostDraw();
}