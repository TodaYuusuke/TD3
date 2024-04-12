#include "PlayerHP.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void PlayerHP::Initialize()
{
	// 下の画像を設定

	// 画像を設定
	sprBackHp_.texture = LoadTexture("white.png");
	sprBackHp_.anchorPoint = { 0.0f,0.0f };
	sprBackHp_.isUI = true;
	sprBackHp_.isActive = true;
	// サイズを取得してサイズを調整できるようにする
	// 指定したサイズでのスケール変更ができるようにする
	kHPBackConvertScale_.x = (kHPBaseSize_.x + kHPBackPlus_.x) / sprBackHp_.texture.t->GetTextureSize().x;
	kHPBackConvertScale_.y = (kHPBaseSize_.y + kHPBackPlus_.y) / sprBackHp_.texture.t->GetTextureSize().y;
	// 指定したサイズに修正
	sprBackHp_.transform.scale = kHPBackConvertScale_;
	sprBackHp_.name = "HPBackSprite";
	sprBackHp_.commonColor = new Utility::Color(0x111111FF);

	// 画像を設定
	sprHp_.texture = LoadTexture("white.png");
	sprHp_.anchorPoint = { 0.0f,0.0f };
	sprHp_.isUI = true;
	sprHp_.isActive = true;
	// サイズを取得してサイズを調整できるようにする
	// 指定したサイズでのスケール変更ができるようにする
	kHPSpriteConvertScale_.x = kHPBaseSize_.x / sprHp_.texture.t->GetTextureSize().x;
	kHPSpriteConvertScale_.y = kHPBaseSize_.y / sprHp_.texture.t->GetTextureSize().y;
	// 指定したサイズに修正
	sprHp_.transform.scale = kHPSpriteConvertScale_;
	sprHp_.transform.translation = { 20.0f,20.0f,0.0f };
	sprHp_.name = "HPSprite";
	hp_ = maxHp_;
	sprHp_.commonColor = new Utility::Color(0x00AA00FF);
	// 画像の下にちょうど来るようにする
	sprBackHp_.transform.translation = sprHp_.transform.translation - kHPBackPlus_ / 2.0f;
	// 一応更新
	sprBackHp_.Update();
	sprHp_.Update();
}

void PlayerHP::Update()
{
	sprBackHp_.transform.scale.x = kHPBackConvertScale_.x * maxHp_;
	sprHp_.transform.scale.x = kHPSpriteConvertScale_.x * hp_;
	// 一応更新
	sprBackHp_.Update();
	sprHp_.Update();
}

void PlayerHP::Increase()
{
	hp_ += hp_ < maxHp_ ? 1 : 0;
}

bool PlayerHP::Decrease()
{
	// 0 になったら true
	return !--hp_;
}

void PlayerHP::IncreaseMax()
{
	maxHp_++;
}

void PlayerHP::DecreaseMax()
{
	maxHp_ -= 1 < maxHp_ ? 1 : 0;
}
