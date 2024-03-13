//#include "Class/Scene/Sample.h"
#include "Game/Scenes/TitleScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp("TD3");
	LWP::System::Run(new TItleScene);
	return 0;
}