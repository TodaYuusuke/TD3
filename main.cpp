//#include "Class/Scene/Sample.h"
#include "Game/Scenes/Title/TitleScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp("TD3");
	LWP::System::Run(new TitleScene);
	return 0;
}