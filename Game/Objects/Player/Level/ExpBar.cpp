#include "ExpBar.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void ExpBar::Initialize()
{
	// 画像を設定
	subSprite1.texture = LoadTexture(GetTexturePass());
	subSprite1.anchorPoint = { 0.5f,0.5f };
	subSprite1.isUI = true;
	subSprite1.isActive = true;
	subSprite1.name = "EXPBackSprite1";
	subSprite1.commonColor = new Utility::Color(0x333333FF);

	// 画像を設定
	subSprite2.texture = LoadTexture(GetTexturePass());
	subSprite2.anchorPoint = { 0.5f,0.5f };
	subSprite2.isUI = true;
	subSprite2.isActive = true;
	subSprite2.name = "EXPBackSprite2";
	subSprite2.commonColor = new Utility::Color(0x888888FF);
	// 画像のサイズを取得
	preSize_ = subSprite2.texture.t.GetSize();

	// 画像を設定
	mainSprite.texture = LoadTexture(GetTexturePass());
	mainSprite.anchorPoint = { 0.5f,0.5f };
	mainSprite.isUI = true;
	mainSprite.isActive = true;
	mainSprite.name = "EXPFrontSprite";
	mainSprite.commonColor = new Utility::Color(0x397C30FF);
	mainSprite.transform.scale.x = 0.5f;
	mainSprite.transform.scale.y = 0.5f;

	// 初期値として設定する
	lwp::Vector3 size{ 1.0f,1.0f,1.0f };
	size.x = kBarSize_.x / preSize_.x;
	size.y = kBarSize_.y / preSize_.y;

	//subSprite1.transform.scale = size * subScale;
	subSprite1.transform.scale.x = (kBarSize_.x + 20.0f) / preSize_.x;
	subSprite1.transform.scale.y = (kBarSize_.y + 20.0f) / preSize_.y;
	//subSprite2.transform.scale = size * subScale;
	subSprite2.transform.scale.x = (kBarSize_.x + 10.0f) / preSize_.x;
	subSprite2.transform.scale.y = (kBarSize_.y + 10.0f) / preSize_.y;
	mainSprite.transform.scale = size;
	mainSprite.transform.scale.x = 0.0f;

	lwp::Vector3 pos{ lwp::GetWindowWidthF() * 0.5f,lwp::GetWindowHeightF() - basePosition.y,0.0f };

	subSprite1.transform.translation = pos;
	subSprite2.transform.translation = pos;
	mainSprite.transform.translation = pos;

	subSprite1.Update();
	subSprite2.Update();
	mainSprite.Update();
}

void ExpBar::Update(uint32_t exp, uint32_t num)
{
	//#ifdef DEMO
	//
	//	ImGui::Begin("EXPBar");
	//
	//	lwp::Vector4 col = mainSprite.commonColor->GetVector4();
	//	if (ImGui::ColorPicker3("color", &col.x))
	//	{
	//		delete mainSprite.commonColor;
	//		mainSprite.commonColor = new Utility::Color(col);
	//	}
	//
	//	ImGui::End();
	//#endif // DEMO


		// 場所と大きさ
	lwp::Vector3 size{ 1.0f,1.0f,1.0f };
	size.x = kBarSize_.x / preSize_.x;

	//subSprite.transform.scale = size * subScale;
	mainSprite.transform.scale.x = size.x * (num / (float)exp);
	//subSprite.transform.scale = { subScale,subScale };
	//subSprite.transform.translation = basePosition;
	//lwp::Vector2 size = subSprite.texture.t.GetSize();
	//mainSprite.transform.translation.x = basePosition.x + size.x * (0.55f - 0.5f)/*上下の隙間*/ * 0.5f/*幅*/;
	//mainSprite.transform.translation.y = basePosition.y + size.y * (0.55f - 0.5f) * 0.5f;

	subSprite1.Update();
	subSprite2.Update();
	mainSprite.Update();
}
