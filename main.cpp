//#include "Class/Scene/Sample.h"
#include "Game/Scenes/Title/TitleScene.h"
#include "Game/Scenes/Clear/ClearScene.h"
#include "Game/Scenes/Game/GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp(L"斬り斬り舞");
#ifdef DEMO
#else
	//LWP::Window::ChangeFullScreenMode();
	LWP::Window::ChangeFullScreenMode();
#endif
	LWP::System::Run(new GameScene);
	return 0;
}