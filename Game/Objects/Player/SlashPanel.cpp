#include "SlashPanel.h"

SlashPanel::~SlashPanel()
{
	for (size_t i = 0; i < katanaPanels_.size(); i++)
	{
		delete katanaPanels_[i];
		delete sayaPanels_[i];
	}
	katanaPanels_.clear();
	sayaPanels_.clear();
}

void SlashPanel::Initialize()
{

	for (size_t i = 0; i < 4; i++)
	{
		UIData data;
		data.fileName_ = "katana128.png";
		data.transform_.y = 800;
		data.anchor_ = { 0.5f,0.5f };
		// 中心からの位置
		data.transform_.x = kWINDOW_WIDTH_HALF_ + 256 * (-1.5f + i);
		// 刀生成
		katanaPanels_.push_back(CreateUIPanel(data));
		// 鞘生成
		data.fileName_ = "saya128.png";
		sayaPanels_.push_back(CreateUIPanel(data));
	}
	index_ = katanaPanels_.size() - 1;
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
	for (size_t i = 0; i < sayaPanels_.size() - 1; i++)
	{
		sayaPanels_[i]->SetIsActive(true);
	}
	sayaPanels_.back()->SetIsActive(false);
	katanaPanels_.back()->SetIsActive(false);
	index_ = katanaPanels_.size() - 1;
}

void SlashPanel::Just()
{
	index_++;
	//sayaPanels_[index_]->SetIsActive(false);
	katanaPanels_.back()->SetIsActive(true);
	sayaPanels_[index_ - 1]->SetIsActive(true);
}

UIPanel* SlashPanel::CreateUIPanel(const UIData& data)
{
	UIPanel* panel = new UIPanel;
	panel->Initialize(data.fileName_);
	panel->SetAnchorPoint(data.anchor_);
	panel->SetPosition(data.transform_);
	return panel;
}
