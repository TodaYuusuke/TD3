#include "SlashPanel.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

std::function<void(int, lwp::Vector3)> SlashPanel::slashResetEffect_ = nullptr;
lwp::Vector3 SlashPanel::psCollect_;

SlashPanel::~SlashPanel()
{
}

void SlashPanel::Initialize()
{
	// 画像を設定
	// とりあえず四角
	subSprite_.texture = LoadTexture("white.png");
	//subSprite_.anchorPoint = { 0.5f,0.5f };
	subSprite_.transform.translation.x = kSPRITEPOSX - 10.0f;
	subSprite_.transform.translation.y = kSPRITEPOSY - 10.0f;
	subSprite_.transform.translation.z = 50.0f;
	subSprite_.isUI = true;
	subSprite_.isActive = true;
	subSprite_.name = "SlSubSprite";
	subSprite_.commonColor = &color1_;

	// 画像を設定
	// とりあえず四角
	mainSprite_.texture = LoadTexture("white.png");
	//mainSprite_.anchorPoint = { 0.5f,0.5f };
	mainSprite_.transform.translation.x = kSPRITEPOSX;
	mainSprite_.transform.translation.y = kSPRITEPOSY;
	mainSprite_.transform.translation.z = 1.0f;
	mainSprite_.isUI = true;
	mainSprite_.isActive = true;
	mainSprite_.name = "SlMainSprite";
	mainSprite_.commonColor = new Utility::Color(Utility::ColorPattern::CYAN);

	// 画像のサイズを取得
	lwp::Vector2 preSize_ = subSprite_.texture.t.GetSize();
	lwp::Vector2 kBarSize_ = { 100.0f,100.0f };
	// 初期値として設定する
	lwp::Vector2 size{ 1.0f,1.0f };
	size.x = kBarSize_.x / preSize_.x;
	size.y = kBarSize_.y / preSize_.y;

	//subSprite1.transform.scale = size * subScale;
	subSprite_.transform.scale.x = (kBarSize_.x + 20.0f) / preSize_.x;
	subSprite_.transform.scale.y = (kBarSize_.y + 20.0f) / preSize_.y;
	//subSprite2.transform.scale = size * subScale;
	mainSprite_.transform.scale.x = (kBarSize_.x) / preSize_.x;
	mainSprite_.transform.scale.y = (kBarSize_.y) / preSize_.y;


	//lwp::Vector2 
	size = mainSprite_.texture.t.GetSize();
	size.x *= mainSprite_.transform.scale.x;
	size.y *= mainSprite_.transform.scale.y;
	// 取得
	msCenter_.x = kSPRITEPOSX + size.x * 0.5f;
	msCenter_.y = kSPRITEPOSY + size.y * 0.5f;
	msCenter_.z = 10.0f;

	// 更新
	subSprite_.Update();
	mainSprite_.Update();

	// パーティクル初期化
	InitParticle();
}

void SlashPanel::Update()
{
	time_ += lwp::GetDeltaTimeF();
	if (kTIME_ <= time_)
	{
		time_ = 0.0f;
		if (mainSprite_.isActive)
			slashResetEffect_(16, msCenter_);
	}
}

void SlashPanel::Slash()
{
	subSprite_.commonColor = &color2_;
	mainSprite_.isActive = false;
	slashResetEffect_(64, lwp::Vector3(msCenter_));
}

void SlashPanel::Reset()
{
	// なぜか毎回呼ばれるけど良いと思う
	subSprite_.commonColor = &color1_;
	mainSprite_.isActive = true;
	slashResetEffect_(64, lwp::Vector3(msCenter_.x, msCenter_.y, -20.0f));
}

void SlashPanel::InitParticle()
{
	static Object::Particle slashResetParticle_;
	slashResetParticle_.SetPrimitive<Primitive::Sprite>();
	slashResetParticle_.P()->isUI = true;
	slashResetParticle_.P()->material.enableLighting = false;
	slashResetParticle_.name = "SlParticle";
	slashResetParticle_.P()->transform.scale = { 0.0001f,0.0001f, 0.0001f };
	slashResetParticle_.P()->commonColor = new Utility::Color(0x882222FF);
	psCollect_ = { 10.0f * -0.5f,10.0f * -0.5f,0.0f };
	slashResetParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};

		newData.wtf = primitive->transform;
		newData.wtf.scale = { 0.1f,0.1f,1.0f };
		newData.wtf.translation += psCollect_;

		// 方向ベクトルを生成
		int dirX = Utility::GenerateRandamNum<int>(-200, 200);
		int dirY = Utility::GenerateRandamNum<int>(-200, 200);
		//int dir3 = Utility::GenerateRandamNum<int>(-200, 200);
		// 速度ベクトル
		Math::Vector3 dir{ dirX, dirY, -1.0f };
		float multiply = Utility::GenerateRandamNum<int>(10, 30) * 0.5f;
		newData.velocity = dir.Normalize() * multiply;

		//for (size_t i = 0; i < primitive->vertices.size(); i++)
		//{
		//	primitive[i].transform.translation.z = -10.0f;
		//}

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
		data->wtf.scale *= 0.92f;
		data->velocity.y -= 0.7f;
		// 速度ベクトルを弱める
		data->velocity *= 0.9f;

		// 一定時間経ったら消す
		return 20 <= data->elapsedFrame ? true : false;
		};
	slashResetParticle_.isActive = true;
	slashResetEffect_ = [&](int i, lwp::Vector3 pos) {
		slashResetParticle_.P()->transform = pos;
		slashResetParticle_.Add(i);
		};
}
