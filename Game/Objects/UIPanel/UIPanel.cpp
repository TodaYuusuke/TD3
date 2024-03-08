#include "UIPanel.h"

void UIPanel::Initialize()
{
	Initialize("resources/system/texture/uvChecker.png");
}

void UIPanel::Initialize(const std::string& fileName)
{
	world_.Initialize();
	// UI
	uiSprite_ = LWP::Primitive::CreateInstance<lwp::Sprite>();
	uiSprite_->texture = lwp::LoadTextureLongPath(fileName);
	uiSprite_->transform.Parent(&world_);
	uiSprite_->isActive = true;
	uiSprite_->name = "UISprite";
	uiSprite_->isUI = true;
}

void UIPanel::Update()
{
}