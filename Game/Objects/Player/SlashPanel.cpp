#include "SlashPanel.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

std::function<void(int, lwp::Vector3)> SlashPanel::slashResetEffect_ = nullptr;

SlashPanel::~SlashPanel()
{
}

void SlashPanel::Initialize()
{
	// 画像を設定
	// とりあえず四角
	subSprite_.texture = LoadTexture("white.png");
	//subSprite_.anchorPoint = { 0.5f,0.5f };
	subSprite_.transform.translation.x = kSPRITEPOSX - 10;
	subSprite_.transform.translation.y = kSPRITEPOSY - 10;
	subSprite_.isUI = true;
	subSprite_.isActive = true;
	subSprite_.name = "SlSubSprite";
	subSprite_.commonColor = new Utility::Color(0x888888FF);

	subSprite_.transform.scale *= 0.5f;

	// 画像を設定
	// とりあえず四角
	mainSprite_.texture = LoadTexture("white.png");
	//mainSprite_.anchorPoint = { 0.5f,0.5f };
	mainSprite_.transform.translation.x = kSPRITEPOSX;
	mainSprite_.transform.translation.y = kSPRITEPOSY;
	mainSprite_.isUI = true;
	mainSprite_.isActive = true;
	mainSprite_.name = "SlMainSprite";
	mainSprite_.commonColor = new Utility::Color(Utility::ColorPattern::CYAN);

	mainSprite_.transform.scale *= 0.4f;

	// 更新
	subSprite_.Update();
	mainSprite_.Update();

	// パーティクル初期化
	InitParticle();
}

void SlashPanel::Update()
{

}

void SlashPanel::Slash()
{
	mainSprite_.isActive = false;
	//slashResetEffect_(16, lwp::Vector3(0.0f, 0.0f, 0.0f));
	slashResetEffect_(16, lwp::Vector3(kSPRITEPOSX, kSPRITEPOSY, 10.0f));
}

void SlashPanel::Reset()
{
	mainSprite_.isActive = true;
}

void SlashPanel::InitParticle()
{
	static Object::Particle slashResetParticle_;
	slashResetParticle_.SetPrimitive<Primitive::Sprite>();
	slashResetParticle_.P()->isUI = true;
	slashResetParticle_.P()->material.enableLighting = false;
	slashResetParticle_.name = "SlParticle";
	//slashResetParticle_.P()->transform.scale = { 0.0001f,0.0001f, 0.0001f };
	slashResetParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::BLACK);
	slashResetParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};

		newData.wtf = primitive->transform;
		newData.wtf.scale = { 0.1f,0.1f,1.0f };

		// 方向ベクトルを生成
		int dirX = Utility::GenerateRandamNum<int>(-200, 200);
		int dirY = Utility::GenerateRandamNum<int>(-200, 200);
		//int dir3 = Utility::GenerateRandamNum<int>(-200, 200);
		// 速度ベクトル
		Math::Vector3 dir{ dirX, dirY, -1.0f };
		float multiply = Utility::GenerateRandamNum<int>(10, 30) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
		};
	// パーティクルの更新処理
	slashResetParticle_.updateFunction = [](Object::ParticleData* data) {
		if (Info::GetDeltaTimeF() == 0.0f)
		{
			return false;
		}

		// 経過秒数追加
		data->elapsedFrame++;

		// 速度ベクトルを加算
		data->wtf.translation += data->velocity;
		// 奥方向に加算
		//data->wtf.translation.z += -9.8f / 80.0f;

		// 少しづつ小さくする
		//data->wtf.scale *= 0.98f;

		// 速度ベクトルを弱める
		//data->velocity *= 0.9f;

		// 一定時間経ったら消す
		return 10 <= data->elapsedFrame ? true : false;
		};
	slashResetParticle_.isActive = true;
	slashResetEffect_ = [&](int i, lwp::Vector3 pos) {
		slashResetParticle_.P()->transform = pos;
		slashResetParticle_.Add(i);
		};
}
