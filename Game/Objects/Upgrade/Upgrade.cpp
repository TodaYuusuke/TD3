#include "Upgrade.h"
#include "Game/Objects/Player/Player.h"
void Upgrade::Init()
{
	pFunc.push_back(DrawOne);
	pFunc.push_back(DrawTwo);
	pFunc.push_back(DrawThree);
	pFunc.push_back(DrawFour);
	pFunc.push_back(DrawFive);
	pFunc.push_back(DrawSix);
	
}

pfunc Upgrade::SelectUpgrade()
{
	RollUpgrade();
		if (LWP::Input::Keyboard::GetTrigger(DIKEYBOARD_1)) {
			selectedNum[select1] = true;
			return pFunc[select1];
		}
		else if (LWP::Input::Keyboard::GetTrigger(DIKEYBOARD_2)) {
			selectedNum[select2] = true;
			return pFunc[select2];
		}
		else if (LWP::Input::Keyboard::GetTrigger(DIKEYBOARD_3)) {
			selectedNum[select3] = true;
			return pFunc[select3];
		}
}

void Upgrade::RollUpgrade()
{
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_int_distribution<int> distribution(0, pFunc.size()-1);

	while (true) {
		int num1 = distribution(randomEngine);
		if (checkUseNum[num1]) {
			continue;
		}
		else {
			select1 = distribution(randomEngine);
			checkUseNum[num1] = true;
			break;
		}
	}
	while (true) {
		int num2 = distribution(randomEngine);
		if (checkUseNum[num2]) {
			continue;
		}
		else {
			select2 = distribution(randomEngine);
			checkUseNum[num2] = true;
			break;
		}
	}
	while (true) {
		int num3 = distribution(randomEngine);
		if (checkUseNum[num3]) {
			continue;
		}
		else {
			select3 = distribution(randomEngine);
			checkUseNum[num3] = true;
			break;
		}
	}
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
void DrawFour()
{
#ifdef _DEBUG
	ImGui::Begin("Upgrade");
	ImGui::Text("4\n");
	ImGui::End();
#endif
}
void DrawFive()
{
#ifdef _DEBUG
	ImGui::Begin("Upgrade");
	ImGui::Text("5\n");
	ImGui::End();
#endif
}
void DrawSix()
{
#ifdef _DEBUG
	ImGui::Begin("Upgrade");
	ImGui::Text("6\n");
	ImGui::End();
#endif
}
