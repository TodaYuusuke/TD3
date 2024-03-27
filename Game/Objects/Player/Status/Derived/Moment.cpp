#include "Moment.h"

#include "Game/Objects/Player/Player.h"


using namespace LWP;
using namespace LWP::Math;


void Moment::Init(Player* p)
{
	assert(p);
	player_ = p;
	// 初期化
	motions_.clear();
}

void Moment::Reset()
{
	elapsedTime_ = 0.0f;
	EndTime_ = player_->config_.Time_.MOMENTBASE_;
}

void Moment::Update()
{
}

void Moment::CreateMotions()
{
}
