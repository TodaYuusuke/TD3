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

	// タイマーの場所
	timerPosition_ = { 1600.0f,100.f,0.0f };

	// タイマーを綺麗に表示する
	countS0_.Initialize();
	countS0_.transform_.translation = timerPosition_;
	countS0_.transform_.translation.x += kPaddingCenter_ + kPaddingNumber_;
	countS0_.isActive_ = true;
	countS1_.Initialize();
	countS1_.transform_.translation = timerPosition_;
	countS1_.transform_.translation.x += kPaddingCenter_;
	countS1_.isActive_ = true;
	countM0_.Initialize();
	countM0_.transform_.translation = timerPosition_;
	countM0_.transform_.translation.x -= kPaddingCenter_;
	countM0_.isActive_ = true;
	countM1_.Initialize();
	countM1_.transform_.translation = timerPosition_;
	countM1_.transform_.translation.x -= kPaddingCenter_ + kPaddingNumber_;
	countM1_.isActive_ = true;
	// 10 分生存
	countM1_.Reset(1);

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
	countS0_.Reset(0);
	countS1_.Reset(0);
	countM0_.Reset(0);
	// 10 分生存
	countM1_.Reset(1);
}

void GameTimer::Update()
{
	DebugWindow();
	checkSec_ += isWatch_ ? GetDeltaTimeF() : 0.0f;
	// 1 秒立っていたら
	if (1.0f < checkSec_)
	{
		currentSec_++;
		checkSec_ = 0.0f;
		// 1 秒経つ
		if (countS0_.Decrease())
		{
			// 10 秒経つ
			if (countS1_.Decrease())
			{
				// 59 秒へ
				countS1_.Reset(5);
				// 1 分経つ
				if (countM0_.Decrease())
				{
					// 10 分経つ
					countM1_.Decrease();
				}
			}
		}
	}
	countS0_.Update();
	countS1_.Update();
	countM0_.Update();
	countM1_.Update();
}

void GameTimer::DebugWindow()
{
	ImGui::Begin("GameTimer");

	if (ImGui::Button("Reset"))
	{
		Reset();
	}
	ImGui::Checkbox("Watch", &isWatch_);
	ImGui::Text("Time   : %d", currentSec_);
	ImGui::Text("Second : %.4f", checkSec_);

	ImGui::Separator();

	if (ImGui::DragFloat2("TimerPos", &timerPosition_.x, 1.0f) ||
		ImGui::DragFloat("PaddingCenter", &kPaddingCenter_) ||
		ImGui::DragFloat("PaddingNumber", &kPaddingNumber_))
	{
		countS0_.transform_.translation = timerPosition_;
		countS0_.transform_.translation.x += kPaddingCenter_ + kPaddingNumber_;
		countS1_.transform_.translation = timerPosition_;
		countS1_.transform_.translation.x += kPaddingCenter_;
		countM0_.transform_.translation = timerPosition_;
		countM0_.transform_.translation.x -= kPaddingCenter_;
		countM1_.transform_.translation = timerPosition_;
		countM1_.transform_.translation.x -= kPaddingCenter_ + kPaddingNumber_;
	}

	ImGui::End();
}
