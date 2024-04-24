#include "HPSprite.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void HPSprite::Initialize()
{
	// 画像を設定
	subSprite.texture = LoadTexture(GetTexturePass());
	subSprite.anchorPoint = { 0.0f,0.0f };
	subSprite.isUI = true;
	subSprite.isActive = true;
	subSprite.name = "HPSubSprite";
	subSprite.commonColor = new Utility::Color(0x888888FF);
	// 画像を設定
	mainSprite.texture = LoadTexture(GetTexturePass());
	mainSprite.anchorPoint = { 0.0f,0.0f };
	mainSprite.isUI = true;
	mainSprite.isActive = true;
	mainSprite.name = "HPMainSprite";
	mainSprite.commonColor = new Utility::Color(0xAF1010FF);
	mainSprite.transform.scale.x = 0.5f;
	mainSprite.transform.scale.y = 0.5f;
	subSprite.Update();
	mainSprite.Update();
}

void HPSprite::Update()
{
	// 場所と大きさ
	subSprite.transform.scale = { subScale,subScale };
	subSprite.transform.translation = basePosition;
	lwp::Vector2 size = subSprite.texture.t.GetSize();
	mainSprite.transform.translation.x = basePosition.x + size.x * (0.55f - 0.5f)/*上下の隙間*/ * 0.5f/*幅*/;
	mainSprite.transform.translation.y = basePosition.y + size.y * (0.55f - 0.5f) * 0.5f;

	subSprite.Update();
	mainSprite.Update();
}
