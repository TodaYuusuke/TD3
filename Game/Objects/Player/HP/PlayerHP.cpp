#include "PlayerHP.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void PlayerHP::Initialize()
{
	// 画像を設定
	sprite_.texture = LoadTexture("uvTexture.png");
	sprite_.anchorPoint = { 0.0f,0.0f };
	sprite_.transform.scale = {};
	sprite_.isUI = true;
	sprite_.isActive = false;
}

void PlayerHP::Update()
{
	// HP が変更されていたら
	if (hp_.GetChanged())
	{

	}
}

void PlayerHP::Increase()
{
	hp_++;
}

void PlayerHP::Decrease()
{
	hp_--;
}

void PlayerHP::IncreaseMax()
{
}

void PlayerHP::DecreaseMax()
{
}
