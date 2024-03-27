#include "Idol.h"

#include "Game/Objects/Player/Player.h"

using namespace LWP;
using namespace LWP::Resource;

void Idol::Init(Player* p)
{
	assert(p);
	player_ = p;

	// アニメーションの初期化と作成
	motions_.clear();
}

void Idol::Reset()
{
	player_->weapon_->SetBehavior(Weapon::Behavior::Root);

	// 時間を初期化
	elapsedTime_ = 0.0f;
	EndTime_ = player_->config_.Time_.ROOTBASE_;


	// アニメーション作成
	//CreateMotions();
}

void Idol::Update()
{
	CheckInputMove();

	// 移動に向かう
	if (isInputMove_)
	{
		player_->RegistStatus(Behavior::Move);
	}

	// 経過時間を加算
	elapsedTime_ += lwp::GetDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Root);
	}
}

void Idol::CreateMotions()
{
}
