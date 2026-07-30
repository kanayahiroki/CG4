#pragma once
#include "Fade.h"
#include "UpData.h"
#include <KamataEngine.h>

class TitleScene {
public:
	// フェーズ（状態）定義
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize();

	// 更新
	void UpDate(); // ※ cppに合わせて大文字Dにしています

	// 描画
	void Draw();

	// タイトル終了判定（main.cpp から呼ばれます）
	bool IsFinished() const { return finished_; }

private:
	// カメラ
	KamataEngine::Camera camera_;

	// 3Dモデル
	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Model* enterModel_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform* worldTransformTitle_ = nullptr;
	KamataEngine::WorldTransform* worldTransformEnter_ = nullptr;

	// 更新処理用クラス・マテリアルカラー
	UpData* upData_ = nullptr;
	KamataEngine::ObjectColor objectColorEnter_;

	// フェード処理・フェーズ管理
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	// アニメーション用変数
	float titleSpeed_ = 1.0f;
	float enterTimer_ = 0.0f;
	float enterAlpha_ = 0.0f;

	// シーン終了フラグ
	bool finished_ = false;
};