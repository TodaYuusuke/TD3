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
		// 刀生成
		katanaPanels_.push_back(CreateUIPanel(data));
		// 鞘生成
		data.fileName_ = "saya128.png";
		sayaPanels_.push_back(CreateUIPanel(data));
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
	if (4 <= sayaPanels_.size())
	{
		/*for (size_t i = sayaPanels_.size(); 4 < i; i--)
		{	
			delete sayaPanels_.back();
			sayaPanels_.pop_back();
			delete katanaPanels_.back();
			katanaPanels_.pop_back();
		}
		*/
		int num = sayaPanels_.size() - 3;
		sayaPanels_.erase(sayaPanels_.end() - num, sayaPanels_.end());
		katanaPanels_.erase(katanaPanels_.end() - num, katanaPanels_.end());
	}

		for (size_t i = 0; i < sayaPanels_.size(); i++)
		{
			sayaPanels_[i]->SetIsActive(true);
		}
	index_ = sayaPanels_.size();
}

void SlashPanel::Just()
{
	UIData data;
	data.fileName_ = "katana128.png";
	data.transform_.y = 800;
	data.anchor_ = { 0.5f,0.5f };
	// 中心からの位置
	data.transform_.x = kWINDOW_WIDTH_HALF_ + 256 * (-1 + katanaPanels_.size());
	// 刀生成
	katanaPanels_.push_back(CreateUIPanel(data));
	// 鞘生成
	data.fileName_ = "saya128.png";
	sayaPanels_.push_back(CreateUIPanel(data));
	index_++;
	sayaPanels_[index_]->SetIsActive(false);
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
