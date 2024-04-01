#include "PlayerParametaer.h"


void PlayerParameter::Initialize(PlayerConfig* p)
{
	assert(p);
	config_ = p;
	ResetParameter();
}

void PlayerParameter::ApplyUpgrade()
{
	// 攻撃力
	power_ = (config_->Power_.BASEPOWER_ + param.power.base) * (0.01f * param.power.percent);
	// 攻撃範囲
	attackRange_ = (config_->Length_.WEAPONCOLLISIONRADIUS_ + param.attackRange.base) * (0.01f * param.attackRange.percent);
	// 移動速度
	moveSpeed = (config_->Speed_.MOVE_ + param.speed.base) * (0.01f * param.speed.percent);
	slashSpeed = (config_->Speed_.SLASH_ + param.speed.base) * (0.01f * param.speed.percent);
	momentSpeed = (config_->Speed_.MOMENT_ + param.speed.base) * (0.01f * param.speed.percent);
	// 攻撃回数
	slashNum = std::max<int>(config_->Count_.SLASHRELATIONMAX_ + param.attackTotal, 1);
}

void PlayerParameter::ApplyUpgrade(const UpgradeParameter& para)
{
	param = para;
	ApplyUpgrade();
}

void PlayerParameter::ResetParameter()
{
	hp_ = config_->Count_.BASEHP_;
	power_ = config_->Power_.BASEPOWER_;
	attackRange_ = config_->Length_.WEAPONCOLLISIONRADIUS_;

	moveSpeed = (config_->Speed_.MOVE_);
	slashSpeed = (config_->Speed_.SLASH_);
	momentSpeed = (config_->Speed_.MOMENT_);

	slashNum = config_->Count_.SLASHRELATIONMAX_;
}