#include "Upgrade.h"

void UpgradeManager::Init()
{
	pFunc.push_back(DrawOne);
	pFunc.push_back(DrawTwo);
	pFunc.push_back(DrawThree);
}

void UpgradeManager::Update()
{
	
}

void DrawOne()
{
#ifdef _DEBUG
	ImGui::Begin("Upgrade");
	ImGui::Text("1\n");
	ImGui::End();
#endif
}
void DrawTwo()
{
#ifdef _DEBUG
	ImGui::Begin("Upgrade");
	ImGui::Text("2\n");
	ImGui::End();
#endif
}
void DrawThree()
{
#ifdef _DEBUG
	ImGui::Begin("Upgrade");
	ImGui::Text("3\n");
	ImGui::End();
#endif
}
