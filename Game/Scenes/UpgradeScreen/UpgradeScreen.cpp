#include "UpgradeScreen.h"

UpgradeScreen::~UpgradeScreen()
{
}

void UpgradeScreen::Initialize()
{
	panels_ = std::make_unique<UpgradePanels>();
	panels_->Initialize();

	pInput_ = new UpgradeInput();
	pInput_->AssignCommands();
}

void UpgradeScreen::Update()
{
	UpdateInput();

#ifdef DEMO
	DebugWindow();
#endif // DEMO
}

void UpgradeScreen::Upgrade()
{
	isUpgrade_ = true;
	panels_->Start();
}

void UpgradeScreen::End()
{
	isUpgrade_ = false;
	panels_->End();
}

void UpgradeScreen::UpdateInput()
{
	// コマンドを積み重ねたものを取得
	pCommands_ = pInput_->HandleInput();

	// コマンドを実行
	// 実際には情報を一度すべて受け取る
	for (std::list<IUpgradeCommand*>::iterator itr = pCommands_->begin();
		itr != pCommands_->end(); ++itr)
	{
		(*itr)->Exec(*this);
	}
}

void UpgradeScreen::DebugWindow()
{
	ImGui::Begin("Upgrade");

	ImGui::Text("How To Controll");
	ImGui::Bullet();
	ImGui::Text("WS | UPDOWN           : MOVE(Not implemented)");
	ImGui::Bullet();
	ImGui::Text("SPACE | ENTER or ABXY : SELECT");

	ImGui::End();
}
