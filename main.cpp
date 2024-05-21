//#include "Class/Scene/Sample.h"
#include "Game/Scenes/Title/TitleScene.h"
#include "Game/Scenes/Clear/ClearScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp(L"斬り斬り舞");
#ifdef DEMO
#else
	LWP::Window::ChangeFullScreenMode();
#endif
	LWP::System::Run(new TitleScene);
	return 0;
}