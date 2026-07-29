#include "GameScene.h"
#include "Math.h"
#include "Model2.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <numbers>
#include <random>

// ★ using namespace MathUtility; は削除しました！
using namespace KamataEngine;

std::random_device seedGenerator;
std::mt19937 randomEngine(seedGenerator());
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

GameScene::~GameScene() {
	delete model_;
	model_ = nullptr;

	// 3Dモデルデータの解放
	delete modelParticle_;

	Model2::StaticFinalize();

	for (auto& e : effects_) {
		delete e.worldTransform;
		e.worldTransform = nullptr;
	}
	effects_.clear();

	for (Particle* particle : particles_) {
		delete particle;
	}
	particles_.clear();
}

void GameScene::Initialize() {
	srand((unsigned int)time(nullptr));

	textureHandle_ = TextureManager::Load("uvChecker.png");

	Model2::StaticInitialize();

	// モデル生成
	model_ = Model2::CreateSquare();

	for (int g = 0; g < 5; g++) {
		Vector3 pos = {(float)(rand() % 200 - 100) / 10.0f, (float)(rand() % 200 - 100) / 10.0f, 0.0f};

		for (int i = 0; i < 15; i++) {
			CreateEffect(pos);
		}
	}

	// 3Dモデルデータの生成
	modelParticle_ = Model::CreateSphere(4, 4);

	// パーティクルの生成
	particle_ = new Particle();

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -20.0f};

	WorldTransform* worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();

	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() {
	camera_.UpdateMatrix();

	for (size_t i = 0; i < effects_.size();) {
		auto& e = effects_[i];

		e.currentTime++;

		e.worldTransform->rotation_.z += 0.1f;

		// フェードアウト
		e.alpha = 1.0f - (float(e.currentTime) / float(e.lifeTime));

		// 更新
		upData_->WorldTransformUpData(*e.worldTransform);
		e.worldTransform->TransferMatrix();

		// 寿命終了
		if (e.currentTime >= e.lifeTime) {
			delete e.worldTransform;
			e.worldTransform = nullptr;
			effects_.erase(effects_.begin() + i);
		} else {
			i++;
		}
	}

	// 全部消えたら一気に再生成
	if (effects_.empty()) {
		for (int g = 0; g < 5; g++) {
			Vector3 position = {(float)(rand() % 200 - 100) / 10.0f, (float)(rand() % 200 - 100) / 10.0f, 0.0f};

			for (int i = 0; i < 15; i++) {
				CreateEffect(position);
			}
		}
	}

	if (rand() % 20 == 0) {
		Vector3 position = {distribution(randomEngine) * 20.0f, distribution(randomEngine) * 20.0f, 0.0f};
		ParticleBorn(position);
	}

	// パーティクルの更新
	for (Particle* particle : particles_) {
		particle->UpDate();
	}

	particles_.remove_if([](Particle* particle) {
		if (particle->IsFinished()) {
			delete particle;
			return true;
		}
		return false;
	});
}

void GameScene::CreateEffect(Vector3 position) {
	effects_.emplace_back();
	EffectData& effect = effects_.back();

	effect.worldTransform = new WorldTransform();
	effect.worldTransform->Initialize();

	float angle = (float)(rand() % 360) * (std::numbers::pi_v<float> / 180.0f);
	effect.worldTransform->rotation_.z = angle;

	float length = (rand() % 200) / 20.0f + 1.0f;
	effect.worldTransform->scale_ = {0.01f, length, 1.0f};
	effect.worldTransform->translation_ = position;

	float speed = (rand() % 100) / 500.0f + 0.02f;
	effect.velocity = {cosf(angle) * speed, sinf(angle) * speed, 0.0f};

	effect.scaleSpeed = 0.01f;
	effect.lifeTime = 40;
	effect.currentTime = 0;
	effect.alpha = 1.0f;

	effect.colorData.Initialize();
	effect.colorData.SetColor({(float)(rand() % 256) / 255.0f, (float)(rand() % 256) / 255.0f, (float)(rand() % 256) / 255.0f, 1.0f});
}

// パーティクル発生 (★演算子エラーが起きないように修正済)
void GameScene::ParticleBorn(Vector3 position) {
	for (int i = 0; i < 50; i++) {
		Particle* particle = new Particle();

		Vector3 velocity = {distribution(randomEngine), distribution(randomEngine), 0.0f};

		velocity = Normalize(velocity);

		float speed = distribution(randomEngine) * 0.1f;
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;

		particle->Initialize(modelParticle_, position, velocity);
		particles_.push_back(particle);
	}
}

void GameScene::Draw() {
	Model::PreDraw();

	for (Particle* particle : particles_) {
		particle->Draw(camera_);
	}

	Model::PostDraw();
}