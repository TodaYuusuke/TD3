//#include "Class/Scene/Sample.h"
#include "Game/Scenes/Title/TitleScene.h"
#include "Game/Scenes/Clear/ClearScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp("TD3");
	LWP::Window::ChangeFullScreenMode();
	LWP::System::Run(new TitleScene);
	return 0;
}