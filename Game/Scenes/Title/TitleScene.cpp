#include "TitleScene.h"

#include "Game/Scenes/Game/GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void TitleScene::Initialize()
{

}

// 更新
void TitleScene::Update()
{
	ImGui::Begin("Scene");
	ImGui::Text("Title");
	ImGui::Text("Space");
	ImGui::End();

	if (Keyboard::GetTrigger(DIK_SPACE))
	{
		nextSceneFunction = []() {return new GameScene; };
	}
}