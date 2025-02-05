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
	EndTime_ = player_->parameter_.Time.momentTime_;
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
		player_->demoModel_.transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

		// パラメータも使う
		moveVector *= player_->parameter_.Speed.moment_ * (float)lwp::GetDeltaTime();

		lwp::Vector3 nextPoint = player_->demoModel_.transform.translation + moveVector;
		float length = sqrt(nextPoint.x * nextPoint.x + nextPoint.z * nextPoint.z);
		if (length < Player::MoveMax) {
			player_->demoModel_.transform.translation += moveVector;
		}
		else if (length > Player::MoveMax) {
			nextPoint = nextPoint.Normalize();
			player_->demoModel_.transform.translation = nextPoint * Player::MoveMax;
		}
	}

	// 経過時間を加算
	elapsedTime_ += lwp::GetDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Root);
		// UI に反映
		player_->slashPanel_->Reset();
	}
}

void Moment::CreateMotions()
{
}
