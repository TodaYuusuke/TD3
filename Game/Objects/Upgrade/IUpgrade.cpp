#include "IUpgrade.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void L::IUpgrade::Init()
{
	// スプライトを生成
	sprite_ = LWP::Primitive::Sprite();
	// 画像を設定
	sprite_.texture = LoadTexture(GetTexturePass());
	sprite_.anchorPoint = { 0.5f,0.5f };
	sprite_.isUI = true;
	sprite_.isActive = false;
}

void L::IUpgrade::Update()
{
	// スプライトの描画を消す
	sprite_.isActive = false;
}

void L::IUpgrade::ShowUI(const LWP::Math::Vector2& pos)
{
	// スプライトを描画させる
	sprite_.isActive = true;
	sprite_.transform.translation.x = pos.x;
	sprite_.transform.translation.y = pos.y;
}
