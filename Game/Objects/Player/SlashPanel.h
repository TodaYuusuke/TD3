#pragma once

#include "Adapter.h"

/// <summary>
/// プレイヤーの居合を可視化する管理クラス
/// </summary>
class SlashPanel
{
private:

	const int32_t kWINDOW_WIDTH_HALF_ = 1980 / 2;
	const float kSPRITEPOSX = kWINDOW_WIDTH_HALF_ + 730.0f;
	const float kSPRITEPOSY = 880.0f;

public:

	// デストラクタ
	~SlashPanel();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// パーティクルを初期化
	static void InitParticle();

public: //*** 外部から参照できる ***//

	// 消費
	void Slash();
	// リセット
	void Reset();

private: //*** プライベート変数 ***//

	// サブアイコン
	LWP::Primitive::Sprite subSprite_;
	// メインアイコン
	LWP::Primitive::Sprite mainSprite_;

	// 画像の色
	LWP::Utility::Color color1_ = 0x888888FF;
	LWP::Utility::Color color2_ = 0x555555FF;

	// 画像の中心
	lwp::Vector3 msCenter_;


	// 数秒に一回パーティクル
	float time_ = 0.0f;
	const float kTIME_ = 0.5f;


	// パーティクル画像の中心までの補正
	static lwp::Vector3 psCollect_;

	// 居合が回復したときのエフェクト
	static std::function<void(int, lwp::Vector3)> slashResetEffect_;

	// パーティクル
	//LWP::Object::Particle slashResetParticle_;

private: // プライベート関数


};

