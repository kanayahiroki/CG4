#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "Effect.h"
#include "UpData.h"

using namespace KamataEngine;

class GameScene {
public:
	~GameScene();

	void Initialize();

	void UpDate();

	void Draw();

	void ParticleBorn(Vector3 position);

private:
	uint32_t textureHandle_ = 0;
	//Effect* model_ = nullptr;
	WorldTransform worldTransform_;
	Camera camera_;
	UpData* upData_ = nullptr;

	// ⭕ 固定配列（10個）に戻します
	static const int maxEffect = 10;

	Effect* models_[maxEffect] = {nullptr};

	WorldTransform worldTransforms_[maxEffect];

	// ⭕ 10個それぞれが「今画面に表示されているか（生きているか）」のフラグ
	bool isAlives_[maxEffect] = {true};

	// ⭕ 10個それぞれの寿命を個別に数えるタイマー
	//float effectCounters_[maxEffect] = {0.0f};
};