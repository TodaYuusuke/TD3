#include "GameTimer.h"

using namespace LWP;
using namespace LWP::Info;

GameTimer* GameTimer::GetInstance()
{
	static GameTimer instance;
	return &instance;
}

void GameTimer::Initialize()
{
	isWatch_ = false;
	checkSec_ = 0.0f;
	currentSec_ = 0u;
}

void GameTimer::Start()
{
	isWatch_ = true;
}

void GameTimer::Stop()
{
	isWatch_ = false;
}

void GameTimer::Reset()
{
	isWatch_ = true;
	checkSec_ = 0.0f;
	currentSec_ = 0u;
}

void GameTimer::Update()
{
	DebugWindow();
	checkSec_ += isWatch_ ? GetDeltaTimeF() : 0.0f;
	currentSec_ += 1.0f < checkSec_ ? 1u : 0u;
	checkSec_ = 1.0f < checkSec_ ? 0.0f : checkSec_;
}

void GameTimer::DebugWindow()
{
	ImGui::Begin("GameTimer");

	ImGui::Checkbox("Watch", &isWatch_);
	ImGui::Text("Time   : %d", currentSec_);
	ImGui::Text("Second : %.4f", checkSec_);

	ImGui::End();
}
