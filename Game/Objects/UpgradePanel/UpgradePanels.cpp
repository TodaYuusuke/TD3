#include "UpgradePanels.h"

UpgradePanels::~UpgradePanels()
{
}

void UpgradePanels::Initialize()
{
	// 背景
	backPanel_ = std::make_unique<UpgradeBackPanel>();
	backPanel_->Initialize();
	backPanel_->SetIsActive(false);

	// アップグレード
	itemPanel0_ = std::make_unique<UpgradeItemPanel>();
	itemPanel0_->Initialize();
	itemPanel0_->SetIsActive(false);
	itemPanel0_->SetPositionX(0.0f);
	itemPanel0_->SetPositionY(100.0f);

	// アップグレード
	itemPanel1_ = std::make_unique<UpgradeItemPanel>();
	itemPanel1_->Initialize();
	itemPanel1_->SetIsActive(false);
	itemPanel1_->SetPositionX(0.0f);
	itemPanel1_->SetPositionY(400.0f);
	// アップグレード
	itemPanel2_ = std::make_unique<UpgradeItemPanel>();
	itemPanel2_->Initialize();
	itemPanel2_->SetIsActive(false);
	itemPanel2_->SetPositionX(0.0f);
	itemPanel2_->SetPositionY(700.0f);

}

void UpgradePanels::Update()
{

}

void UpgradePanels::Start()
{
	backPanel_->SetIsActive(true);
	itemPanel0_->SetIsActive(true);
	itemPanel2_->SetIsActive(true);
	itemPanel1_->SetIsActive(true);
}

void UpgradePanels::End()
{
	backPanel_->SetIsActive(false);
	itemPanel0_->SetIsActive(false);
	itemPanel1_->SetIsActive(false);
	itemPanel2_->SetIsActive(false);
}
