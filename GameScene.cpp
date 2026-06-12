#include "GameScene.h"
#include <cstdlib>
#include <ctime>
#include <numbers>

GameScene::~GameScene() { Effect::StaticFinalize(); }

void GameScene::Initialize() {
	srand((unsigned int)time(nullptr));
	textureHandle_ = TextureManager::Load("uvChecker.png");
	Effect::StaticInitialize();
	model_ = Effect::CreateEffect();

	// 10個のトゲトゲを初期化
	for (int i = 0; i < maxEffect; i++) {
		worldTransforms_[i].Initialize();

		// ランダムな角度をセット
		float degres = (float)(rand() % 360);
		float radius = degres * (std::numbers::pi_v<float> / 180.0f);
		worldTransforms_[i].rotation_.z = radius;

		// 最初の状態はみんな「生きている」
		isAlives_[i] = true;
		effectCounters_[i] = 0.0f;
	}

	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};
	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() {

	for (int i = 0; i < maxEffect; i++) {
		//  すでに死んでいるやつは何もしないでスキップ（これが削除の代わり！）
		if (!isAlives_[i]) {
			continue;
		}

		model_->Update();

		// タイマーをフレーム単位で進める（60フレーム = 1秒）
		effectCounters_[i] += 1.0f;

		// 1秒経ったら「死亡フラグ」を立てて、以降の更新をスキップ！
		if (effectCounters_[i] >= 60.0f) {
			isAlives_[i] = false; //  これが「不要なオブジェクトの削除」に相当します！
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

	for(int i = 0; i < maxEffect; i++) 
	{

		if (!isAlives_[i]) {
			continue;
		}

		model_->Draw(worldTransforms_[i], camera_, textureHandle_);
	}

	// Model描画終了
	Effect::PostDraw();
}