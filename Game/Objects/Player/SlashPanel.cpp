#include "SlashPanel.h"

void SlashPanel::Initialize()
{

	for (size_t i = 0; i < 3; i++)
	{
		UIData data;
		data.fileName_ = "katana128.png";
		data.transform_.y = 800;
		data.anchor_ = { 0.5f,0.5f };
		// 中心からの位置
		data.transform_.x = kWINDOW_WIDTH_HALF_ + 256 * (-1 + i);
		// 刀製造
		katanaPanels_.emplace_back(CreateUIPanel(data));
		// 鞘製造
		data.fileName_ = "saya128.png";
		sayaPanels_.emplace_back(CreateUIPanel(data));
	}
	index_ = katanaPanels_.size();
}

void SlashPanel::Update()
{

}

void SlashPanel::Slash()
{
	sayaPanels_[index_ - 1]->SetIsActive(false);
	index_--;
}

void SlashPanel::Reset()
{
	for (size_t i = 0; i < sayaPanels_.size(); i++)
	{
		sayaPanels_[i]->SetIsActive(true);
	}
	index_ = sayaPanels_.size();
}

UIPanel* SlashPanel::CreateUIPanel(const UIData& data)
{
	UIPanel* panel = new UIPanel;
	panel->Initialize(data.fileName_);
	panel->SetAnchorPoint(data.anchor_);
	panel->SetPosition(data.transform_);
	return panel;
}
