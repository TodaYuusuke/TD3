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
	isEnd_ = false;
	checkSec_ = 0.0f;
	currentSec_ = 0u;
	limitSec_ = 0u;
	// 最大時間を設定
	limitSec_ = 60 * 10;

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
	isActive_ = true;
}

void GameTimer::Stop()
{
	isWatch_ = false;
}

void GameTimer::Reset()
{
	isWatch_ = true;
	isActive_ = false;
	isEnd_ = false;
	checkSec_ = 0.0f;
	currentSec_ = 0u;
	// 分
	int m = limitSec_ / 60;
	// 秒
	int s = limitSec_ % 60;
	countM1_.Reset(m / 10);
	countM0_.Reset(m % 10);
	countS1_.Reset(s / 10);
	countS0_.Reset(s % 10);
}

void GameTimer::Reset(uint32_t limit)
{
	limitSec_ = limit;
	Reset();
}

void GameTimer::Update()
{

	countS0_.isActive_ =
		countS1_.isActive_ =
		countM0_.isActive_ =
		countM1_.isActive_ = isActive_;
	// 終わっていたらカウントしない
	isWatch_ = isEnd_ ? false : isWatch_;

	DebugWindow();
	checkSec_ += isWatch_ ? GetDeltaTimeF() : 0.0f;
	// 1 秒立っていたら
	if (1.0f < checkSec_)
	{
		currentSec_++;
		checkSec_ = 0.0f;
		limitSec_--;
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
	if (limitSec_ == 0u)
	{
		isEnd_ = true;
	}
	countS0_.Update();
	countS1_.Update();
	countM0_.Update();
	countM1_.Update();
}

void GameTimer::DebugWindow()
{
#ifdef DEMO

	ImGui::Begin("GameTimer");

	if (ImGui::Button("Reset"))
	{
		Reset();
	}
	ImGui::Checkbox("Active", &isActive_);
	ImGui::Checkbox("Watch", &isWatch_);
	ImGui::Checkbox("End", &isEnd_);
	ImGui::Text("Time   : %d", currentSec_);
	ImGui::Text("Limit  : %d", limitSec_);
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

#endif // DEMO
}
