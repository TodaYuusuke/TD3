#include "ISkill.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;

void L::ISkill::Init()
{
	// スプライトを生成
	//sprite_ = LWP::Primitive::Sprite();
	// 画像を設定
	sprite_.texture = LoadTexture(GetTexturePass());
	sprite_.anchorPoint = { 0.5f,0.5f };
	sprite_.isUI = true;
	sprite_.isActive = false;
	// フラグを false
	isApplied = false;
}

void L::ISkill::Update()
{
	// スプライトの描画を消す
	sprite_.isActive = false;
}

void L::ISkill::ShowUI(const LWP::Math::Vector2& pos)
{
	// スプライトを描画させる
	sprite_.isActive = true;
	sprite_.transform.translation.x = pos.x;
	sprite_.transform.translation.y = pos.y;
}

void L::ISkill::DebugTree()
{
	if (ImGui::TreeNode(GetUpgradeName().c_str()))
	{
		for (IUpgrade* up : attackUpgrades_)
		{
			ImGui::Text(up->GetUpgradeName().c_str());
		}
		ImGui::TreePop();
		ImGui::Separator();
	}
}