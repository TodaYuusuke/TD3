#include "Move.h"

#include "Game/Objects/Player/Player.h"


using namespace LWP;
using namespace LWP::Math;


void Move::Init(Player* p)
{
	assert(p);
	player_ = p;

	// アニメーションの初期化と作成
	motions_.clear();
}

void Move::Reset()
{
	// 時間を初期化
	elapsedTime_ = 0.0f;
	EndTime_ = player_->config_.Time_.MOVEBASE_;

	// アニメーション作成
	CreateMotions();
}

void Move::Update()
{
	// 居合するか
	if (player_->flag_.isInputSlash_)
	{
		player_->RegistStatus(Behavior::Slash);
	}
	// 移動方向をカメラに合わせる
	lwp::Vector3 moveVector = player_->GetVectorTranspose(player_->destinate_);

	// モデル回転
	player_->demoModel_.transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

	// パラメータも使う
	moveVector *= player_->parameter_.Speed.move_ * lwp::GetDeltaTimeF();

	player_->demoModel_.transform.translation += moveVector;
	// 滑らか用
	//player_->rootData_.velocity_ = moveVector.Length() != 0.0f ? moveVector : player_->rootData_.velocity_;

	// 経過時間を加算
	elapsedTime_ += lwp::GetDeltaTimeF();
	if (EndTime_ <= elapsedTime_)
	{
		player_->RegistStatus(Behavior::Root);
	}
}

void Move::CreateMotions()
{
}