#include "UIPanel.h"

UIPanel::UIPanel(const UIData& data)
{
	Initialize(data);
}

void UIPanel::Initialize()
{
	Initialize("../system/texture/uvChecker.png");
}

void UIPanel::Initialize(const std::string& fileName)
{
	world_.Initialize();
	// UI
	uiSprite_ = new LWP::Primitive::Sprite();
	uiSprite_->texture = lwp::LoadTexture(fileName);
	uiSprite_->transform.Parent(&world_);
	uiSprite_->isActive = true;
	uiSprite_->name = "UISprite";
	uiSprite_->isUI = true;
}

void UIPanel::Initialize(const UIData& data)
{
	this->Initialize(data.fileName_);
	this->SetAnchorPoint(data.anchor_);
	this->SetPosition(data.transform_);
}

void UIPanel::Update()
{
}