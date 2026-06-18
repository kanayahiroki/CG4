#include "GameScene.h"
#include <cstdlib>
#include <ctime>
#include <numbers>
#include <random>

GameScene::~GameScene() { Effect::StaticFinalize(); }

std::random_device seedGenerator; // 非決定的な乱数生成器
std::mt19937 randomEngine(seedGenerator()); // メルセンヌ・ツイスターの乱数エンジン
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f); // -1.0f から 1.0f までの一様分布

void GameScene::Initialize() {
	srand((unsigned int)time(nullptr));
	textureHandle_ = TextureManager::Load("uvChecker.png");
	Effect::StaticInitialize();
	//model_ = Effect::CreateEffect();

	for (int i = 0; i < maxEffect; i++) 
	{
		models_[i] = Effect::CreateEffect();
	}
	

	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};
	upData_ = new UpData();
	assert(upData_);


	Vector3 position = {0.0f,0.0f,0.0f};

	ParticleBorn(position);
}

void GameScene::UpDate() {

	for (int i = 0; i < maxEffect; i++) {
		//  すでに死んでいるやつは何もしないでスキップ（これが削除の代わり！）
		if (!isAlives_[i]) {
			continue;
		}

		//  10個のトゲトゲを回転させる
		models_[i]->Update();

		//  すでに寿命が尽きていたら死んだことにする
		if (models_[i]->IsFinished()) {
			isAlives_[i] = false;
			continue;
		}

		// 生きているやつだけ行列を更新して動かす
		upData_->WorldTransformUpData(worldTransforms_[i]);
	}
	camera_.UpdateMatrix();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// Model描画開始
	Effect::PreDraw(commandList);

	for (int i = 0; i < maxEffect; i++) {

		if (!isAlives_[i]) {
			continue;
		}

		models_[i]->Draw(worldTransforms_[i], camera_, textureHandle_);
	}

	// Model描画終了
	Effect::PostDraw();
}

void GameScene::ParticleBorn(Vector3 position) 
{// 10個のトゲトゲを初期化
	for (int i = 0; i < maxEffect; i++) {
		worldTransforms_[i].Initialize();

		// ランダムな位置をセット
		worldTransforms_[i].translation_ = position;

		// ランダムな角度をセット
		float degres = (float)(rand() % 360);
		float radius = degres * (std::numbers::pi_v<float> / 180.0f);
		worldTransforms_[i].rotation_.z = radius;

	

		models_[i]->counter_ = 0.0f;
		models_[i]->isFinishied_ = false;

		isAlives_[i] = true;
	}
}
