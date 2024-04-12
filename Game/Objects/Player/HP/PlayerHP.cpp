#include "PlayerHP.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void PlayerHP::Initialize()
{
	// 画像を設定
	sprHp_.texture = LoadTexture("uvTexture.png");
	sprHp_.anchorPoint = { 0.0f,0.0f };
	sprHp_.isUI = true;
	sprHp_.isActive = true;
	// サイズを取得してサイズを調整できるようにする
	// 指定したサイズでのスケール変更ができるようにする
	kHPSpriteConvertScale_.x = kHPBaseSize_.x / sprHp_.texture.t->GetTextureSize().x;
	kHPSpriteConvertScale_.y = kHPBaseSize_.y / sprHp_.texture.t->GetTextureSize().y;
	// 指定したサイズに修正
	sprHp_.transform.scale = kHPSpriteConvertScale_;
	hp_ = maxHp_;
	// 一応更新
	sprHp_.Update();
}

void PlayerHP::Update()
{

	// 一応更新
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
