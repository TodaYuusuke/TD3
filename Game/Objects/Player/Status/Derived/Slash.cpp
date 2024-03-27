#include "Slash.h"

#include "Game/Objects/Player/Player.h"


using namespace LWP;
using namespace LWP::Math;


void Slash::Init(Player* p)
{
	assert(p);
	player_ = p;

	// アニメーションの初期化と作成
	motions_.clear();
}

void Slash::Reset()
{
	player_->weapon_->SetBehavior(Weapon::Behavior::Slash);
	// 時間を初期化
	elapsedTime_ = 0.0f;
	EndTime_ = player_->config_.Time_.SLASHBASE_;



	// アニメーション作成
	CreateMotions();
}

void Slash::Update()
{
	CheckInputMove();
	// 一定方向を向く
	lwp::Vector3 moveVector = player_->slashData_.vector_;

	// モデル回転
	player_->demoModel_->transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	moveVector = moveVector * (player_->config_.Speed_.SLASH_ / player_->slashData_.maxTime_) * (float)lwp::GetDeltaTime();

	player_->demoModel_->transform.translation += moveVector;	// 無敵時間
	// ジャスト成立中
	//　無敵時間中
	//colliders_.player_->isActive = (!flag_.isJustSlashing_ && config_.Time_.JUSTTAKETIME_ + config_.Time_.JUSTINVINCIBLE_ < t);
	//flag_.isInvincible_ = (!flag_.isJustSlashing_ && config_.Time_.JUSTTAKETIME_ + config_.Time_.JUSTINVINCIBLE_ < t);
	// 判定を取れるようにする
	player_->colliders_.justSlash_->isActive = elapsedTime_ < player_->config_.Time_.JUSTTAKETIME_;

	// 武器の判定を伸ばす
	player_->colliders_.weapon_->end = player_->demoModel_->transform.translation + player_->slashData_.vector_ * player_->config_.Length_.WEAPONPLUSCORRECTION_;


	elapsedTime_ += lwp::GetDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Moment);
	}
}

void Slash::CreateMotions()
{
}
