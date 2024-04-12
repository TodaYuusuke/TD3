#include "ClearScene.h"

#include "Game/Scenes/Title/TitleScene.h"
#include "Game/Scenes/Game/GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void ClearScene::Initialize()
{

}

// 更新
void ClearScene::Update()
{
	ImGui::Begin("Scene");
	ImGui::Text("Clear");
	ImGui::End();

	if (Keyboard::GetTrigger(DIK_SPACE))
	{
		nextSceneFunction = []() {return new TitleScene; };
	}
}