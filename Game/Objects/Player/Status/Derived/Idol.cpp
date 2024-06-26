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
	// 時間を初期化
	elapsedTime_ = 0.0f;
	EndTime_ = player_->config_.Time_.ROOTBASE_;

	// 居合回数のリセット
	player_->slashData_.relationSlash_ = 0u;
	// 最大数もリセット
	player_->slashData_.maxRelation_ = player_->parameter_.Attack.slashNum_;

	//slashData_.maxRelation_ = slashData_.cMAXRELATION_;
	//player_->slashData_.maxRelation_ = player_->config_.Count_.SLASHRELATIONMAX_;
	player_->weapon_->SetBehavior(Weapon::Behavior::Root);
	// UI に反映
	player_->slashPanel_->Reset();

	// アニメーション作成
	//CreateMotions();
}

void Idol::Update()
{
	// 移動に向かう
	/*if (player_->flag_.isInputMove_)
	{
		player_->RegistStatus(Behavior::Move);
	}*/
	// 居合するか
	if (player_->flag_.isInputSlash_)
	{
		player_->RegistStatus(Behavior::Slash);
	}
	// 移動入力されていたら
	if (player_->flag_.isInputMove_)
	{
		// 移動方向をカメラに合わせる
		lwp::Vector3 moveVector = player_->GetVectorTranspose(player_->destinate_);

		// モデル回転
		player_->demoModel_.transform.rotation.y = std::atan2f(moveVector.x, moveVector.z);

		// パラメータも使う
		moveVector *= player_->parameter_.Speed.move_ * lwp::GetDeltaTimeF();

		player_->demoModel_.transform.translation += moveVector;
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
