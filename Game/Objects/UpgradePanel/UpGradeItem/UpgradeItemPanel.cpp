#include "UpgradeItemPanel.h"

typedef UIPanel::UIData UIData;

UpgradeItemPanel::~UpgradeItemPanel()
{
	delete panel_;
}

void UpgradeItemPanel::Initialize()
{
	UIData data;
	data.fileName_ = "../system/texture/uvChecker.png";
	data.transform_.x = kWINDOW_WIDTH_HALF_;
	data.transform_.y = kWINDOW_HEIGHT_HALF_;
	data.anchor_ = { 0.5f,0.5f };
	panel_ = new UIPanel(data);
	panel_->SetSize(kWINDOW_WIDTH_HALF_, kWINDOW_HEIGHT_HALF_);
}

void UpgradeItemPanel::Update()
{
}

void UpgradeItemPanel::DebugWindow()
{
}
