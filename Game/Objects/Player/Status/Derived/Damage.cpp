#include "Damage.h"

#include "Game/Objects/Player/Player.h"


using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Resource;


void Damage::Init(Player* p)
{
	assert(p);
	player_ = p;
	// アニメーションの初期化と作成
	motions_.clear();
}

void Damage::Reset()
{
	motions_.clear();

	// 時間を初期化
	elapsedTime_ = 0.0f;
	EndTime_ = player_->config_.Time_.DAMAGEBASE_;

	// アニメーション作成
	//CreateMotions();
}

void Damage::Update()
{
	elapsedTime_ += lwp::GetDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Root);
	}
}

void Damage::CreateMotions()
{
}
