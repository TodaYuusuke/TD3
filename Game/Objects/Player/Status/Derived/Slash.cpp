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
	player_->pCamera_->StartSlash();

	// 居合の方向を更新
	// 居合中には変更しない
	player_->slashData_.vector_ = player_->GetVectorTranspose(player_->destinate_);

	// デルタタイム変更
	player_->EndJust();
	//slashData_.maxTime_ = slashData_.cBASETIME;
	player_->slashData_.maxTime_ = player_->config_.Time_.SLASHBASE_;
	player_->weapon_->SetBehavior(Weapon::Behavior::Slash);
	// 居合回数加算
	player_->slashData_.relationSlash_++;
	// UI に反映
	//player_->slashPanel_->Slash();
	// 当たり判定を消去
	//colliders_.player_->isActive = false;
	player_->flag_.isInvincible_ = true;
	// ジャスト判定中は無敵
	player_->invincibleTime_ = 0.0f;
	player_->maxInvincibleTime_ = player_->config_.Time_.JUSTTAKETIME_ + player_->config_.Time_.JUSTINVINCIBLECORRECTION_;

	// コライダーの設定
	ResetCollider();

	// アニメーション作成
	CreateMotions();
}

void Slash::Update()
{
	// ジャスト中に居合をするか
	if (player_->flag_.isInputSlash_ && player_->flag_.isJustSlashing_)
	{
		player_->RegistStatus(Behavior::Slash);
	}
	// 一定方向を向く
	lwp::Vector3 moveVector = player_->slashData_.vector_ *
		player_->parameter_.slashSpeed * lwp::GetDefaultDeltaTimeF();

	player_->demoModel_.transform.translation += moveVector;

	// 判定を取れるようにする
	player_->colliders_.justSlash_.isActive = elapsedTime_ < player_->config_.Time_.JUSTTAKETIME_;

	// 武器の判定を伸ばす
	player_->colliders_.weapon_.end =
		player_->demoModel_.transform.translation +
		player_->slashData_.vector_ *
		player_->config_.Length_.WEAPONPLUSCORRECTION_;


	elapsedTime_ += lwp::GetDefaultDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Moment);
	}
}

void Slash::CreateMotions()
{
}

void Slash::ResetCollider()
{
	// 武器の当たり判定を出す
	// カプセルの設定
	lwp::Vector3 start = player_->demoModel_.transform.translation;
	lwp::Vector3 end = player_->demoModel_.transform.translation;
	player_->colliders_.weapon_.Create(start, end);
	player_->colliders_.weapon_.radius = player_->config_.Length_.WEAPONCOLLISIONRADIUS_;
	player_->colliders_.weapon_.isActive = true;
	// ジャスト判定を作る
	player_->colliders_.justSlash_.Create(start, end);
	// サイズ
	player_->colliders_.justSlash_.radius = player_->config_.Length_.JUSTCOLLISIONRADIUS_;
	player_->colliders_.justSlash_.end = player_->demoModel_.transform.translation + player_->slashData_.vector_ * (player_->config_.Speed_.SLASH_ * player_->config_.Par_.JUSTENABLE_);
	player_->colliders_.justSlash_.isActive = true;
}
