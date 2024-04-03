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
	base.Attack.slashPower_ = (config_->Power_.BASEPOWER_ + param.Attack.slashPowerDelta.base);
	// 攻撃範囲
	base.Attack.slashRange_ = (config_->Length_.WEAPONCOLLISIONRADIUS_ + param.Attack.slashRangeDelta.base);
	// 移動速度
	base.Speed.move_ = (config_->Speed_.MOVE_ + param.Speed.allSpeedDelta.base);
	base.Speed.slash_ = (config_->Speed_.SLASH_ + param.Speed.allSpeedDelta.base);
	base.Speed.moment_ = (config_->Speed_.MOMENT_ + param.Speed.allSpeedDelta.base);
	// 攻撃回数
	base.Attack.slashNum_ = std::max<int>(config_->Count_.SLASHRELATIONMAX_ + (int)param.Attack.slashNumDelta.base, 1);

	// 掛け算部分の計算
	PlayerParameter multi;

	// 攻撃力
	multi.Attack.slashPower_ = (0.01f * param.Attack.slashPowerDelta.percent);
	// 攻撃範囲
	multi.Attack.slashRange_ = (0.01f * param.Attack.slashRangeDelta.percent);
	// 移動速度
	multi.Speed.move_ = (0.01f * param.Speed.allSpeedDelta.percent);
	multi.Speed.slash_ = (0.01f * param.Speed.allSpeedDelta.percent);
	multi.Speed.moment_ = (0.01f * param.Speed.allSpeedDelta.percent);

	*this = base * multi;
}

void PlayerParameter::ApplyUpgrade(const UpgradeParameter& para)
{
	param = para;
	ApplyUpgrade();
}

void PlayerParameter::ResetParameter()
{
	Hp.hp_ = config_->Count_.BASEHP_;
	
	Attack.slashPower_ = config_->Power_.BASEPOWER_;
	Attack.slashRange_ = config_->Length_.WEAPONCOLLISIONRADIUS_;
	Attack.slashNum_ = config_->Count_.SLASHRELATIONMAX_;

	Speed.move_ = (config_->Speed_.MOVE_);
	Speed.slash_ = (config_->Speed_.SLASH_);
	Speed.moment_ = (config_->Speed_.MOMENT_);

}

PlayerParameter PlayerParameter::operator*(const PlayerParameter& obj)
{
	PlayerParameter temp = *this;
	temp.Attack.slashPower_ = this->Attack.slashPower_ * obj.Attack.slashPower_;
	temp.Attack.slashRange_ = this->Attack.slashRange_ * obj.Attack.slashRange_;
	temp.Speed.move_ = this->Speed.move_ * obj.Speed.move_;
	temp.Speed.slash_ = this->Speed.slash_ * obj.Speed.slash_;
	temp.Speed.moment_ = this->Speed.moment_ * obj.Speed.moment_;
	temp.Attack.slashNum_ = this->Attack.slashNum_;

	temp.config_ = this->config_;
	return temp;
}
