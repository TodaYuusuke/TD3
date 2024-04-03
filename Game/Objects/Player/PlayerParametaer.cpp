#include "PlayerParametaer.h"


void PlayerParameter::Initialize(PlayerConfig* p)
{
	assert(p);
	config_ = p;
	ResetParameter();
}

void PlayerParameter::ApplyUpgrade()
{
	// 足し算の計算
	PlayerParameter base = *this;

	// 攻撃力
	base.slashPower_ = (config_->Power_.BASEPOWER_ + param.slashPowerDelta.base);
	// 攻撃範囲
	base.slashRange_ = (config_->Length_.WEAPONCOLLISIONRADIUS_ + param.slashRangeDelta.base);
	// 移動速度
	base.moveSpeed = (config_->Speed_.MOVE_ + param.allSpeedDelta.base);
	base.slashSpeed = (config_->Speed_.SLASH_ + param.allSpeedDelta.base);
	base.momentSpeed = (config_->Speed_.MOMENT_ + param.allSpeedDelta.base);
	// 攻撃回数
	base.slashNum = std::max<int>(config_->Count_.SLASHRELATIONMAX_ + param.slashDelta.base, 1);

	// 掛け算部分の計算
	PlayerParameter multi;

	// 攻撃力
	multi.slashPower_ = (0.01f * param.slashPowerDelta.percent);
	// 攻撃範囲
	multi.slashRange_ = (0.01f * param.slashRangeDelta.percent);
	// 移動速度
	multi.moveSpeed = (0.01f * param.allSpeedDelta.percent);
	multi.slashSpeed = (0.01f * param.allSpeedDelta.percent);
	multi.momentSpeed = (0.01f * param.allSpeedDelta.percent);

	*this = base * multi;
}

void PlayerParameter::ApplyUpgrade(const UpgradeParameter& para)
{
	param = para;
	ApplyUpgrade();
}

void PlayerParameter::ResetParameter()
{
	hp_ = config_->Count_.BASEHP_;
	slashPower_ = config_->Power_.BASEPOWER_;
	slashRange_ = config_->Length_.WEAPONCOLLISIONRADIUS_;

	moveSpeed = (config_->Speed_.MOVE_);
	slashSpeed = (config_->Speed_.SLASH_);
	momentSpeed = (config_->Speed_.MOMENT_);

	slashNum = config_->Count_.SLASHRELATIONMAX_;
}

PlayerParameter PlayerParameter::operator*(const PlayerParameter& obj)
{
	PlayerParameter temp = *this;
	temp.slashPower_ = this->slashPower_ * obj.slashPower_;
	temp.slashRange_ = this->slashRange_ * obj.slashRange_;
	temp.moveSpeed = this->moveSpeed * obj.moveSpeed;
	temp.slashSpeed = this->slashSpeed * obj.slashSpeed;
	temp.momentSpeed = this->momentSpeed * obj.momentSpeed;
	temp.slashNum = this->slashNum;

	temp.config_ = this->config_;
	return temp;
}
