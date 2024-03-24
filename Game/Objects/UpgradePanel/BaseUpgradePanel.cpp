#include "BaseUpgradePanel.h"

typedef UIPanel::UIData UIData;

BaseUpgradePanel::~BaseUpgradePanel()
{
	delete panel_;
}

void BaseUpgradePanel::Initialize()
{
	UIData data;
	data.fileName_ = "../system/texture/uvChecker.png";
	data.transform_.x = kWINDOW_WIDTH_HALF_;
	data.transform_.y = kWINDOW_HEIGHT_HALF_;
	data.anchor_ = { 0.5f,0.5f };
	panel_ = new UIPanel(data);

}

void BaseUpgradePanel::Update()
{
}

void BaseUpgradePanel::DebugWindow()
{
}
