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
	//EndTime_ = player_->config_.Time_.MOMENTBASE_ + (player_->momentData_.relationSlash_ * player_->config_.Time_.MOMENTINCREMENT_);
	EndTime_ = player_->config_.Time_.MOMENTBASE_;
	// デルタタイム変更
	player_->EndJust();
	player_->momentData_.relationSlash_ = player_->slashData_.relationSlash_;
	// 回数分フレームを加算
	player_->momentData_.maxTime_ = player_->config_.Time_.MOMENTBASE_ + (player_->momentData_.relationSlash_ * player_->config_.Time_.MOMENTINCREMENT_);
	player_->weapon_->SetBehavior(Weapon::Behavior::Moment);
	// 武器の判定を消す
	player_->colliders_.weapon_.isActive = false;
}

void Moment::Update()
{
	// 居合するか
	if (player_->flag_.isInputSlash_)
	{
		player_->RegistStatus(Behavior::Slash);
	}
	// 動いていますか？
	if (player_->flag_.isInputMove_)
	{
		// 移動方向をカメラに合わせる
		lwp::Vector3 moveVector = player_->GetVectorTranspose(player_->destinate_);

		// モデル回転
		player_->demoModel_.worldTF.rotation.y = std::atan2f(moveVector.x, moveVector.z);

		// パラメータも使う
		moveVector *= player_->parameter_.Speed.moment_ * (float)lwp::GetDeltaTime();

		player_->demoModel_.worldTF.translation += moveVector;
	}

	// 経過時間を加算
	elapsedTime_ += lwp::GetDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Root);
	}
}

void Moment::CreateMotions()
{
}
