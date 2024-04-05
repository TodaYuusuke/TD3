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
	// 無敵判定を出す
	player_->flag_.isInvincible_ = true;
	player_->invincibleTime_ = 0.0f;
	player_->maxInvincibleTime_ = player_->config_.Time_.DAMAGEINVINCIBLE_;
	// 武器の判定を消す
	player_->colliders_.weapon_.isActive = false;
	// アニメーション作成
	//CreateMotions();
	// プレイヤーにダメージを与える
	player_->parameter_.Hp.hp_--;
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
