#include "PlayerParametaer.h"


void PlayerParameter::Initialize(PlayerConfig* p)
{
	assert(p);
	config_ = p;
	ResetParameter();
	// 変数の初期化
	Hp.Initialize(config_->Count_.MAXHP_);
}

void PlayerParameter::Update()
{
	Hp.Update();
}

void PlayerParameter::ApplyUpgrade()
{
	ApplyHP();
	ApplyAttack();
	ApplySpeed();
	ApplyTime();
}

void PlayerParameter::ApplyUpgrade(const UpgradeParameter& para)
{
	param = para;
	ApplyUpgrade();
}

void PlayerParameter::ResetParameter()
{
	Hp.hp_ = config_->Count_.BASEHP_;
	Hp.maxHp_ = config_->Count_.MAXHP_;

	Attack.slashPower_ = config_->Power_.BASEPOWER_;
	Attack.slashRange_ = config_->Length_.WEAPONCOLLISIONRADIUS_;
	Attack.slashNum_ = config_->Count_.SLASHRELATIONBASE_;

	Speed.move_ = (config_->Speed_.MOVE_);
	Speed.slash_ = (config_->Speed_.SLASH_);
	Speed.moment_ = (config_->Speed_.MOMENT_);

	Time.invincibleDamage_ = config_->Time_.DAMAGEINVINCIBLE_;
	Time.justTake_ = config_->Time_.JUSTTAKETIME_;
	Time.momentTime_ = config_->Time_.MOMENTBASE_;
}

void PlayerParameter::IncreaseHP()
{
	Hp.Increase();
}

bool PlayerParameter::DecreaseHP()
{
	return Hp.Decrease();
}

void PlayerParameter::IncreaseHPMAX()
{
	Hp.IncreaseMax();
}

void PlayerParameter::DecreaseHPMAX()
{
	Hp.DecreaseMax();
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

void PlayerParameter::ApplyHP()
{
	// 別で用意したクラスに反映
	// ここで回復とかもすればいい
	Hp.maxHp_ = config_->Count_.MAXHP_ + (int)param.HP.hpDelta.base;
	if (Hp.maxHp_ < 1u)
	{
		Hp.maxHp_ = 1u;
	}
	// 最大数が増加したら HP も増加
	if (0.0f < param.HP.hpDelta.base)
	{
		Hp.Increase();
	}

	// HP を最大超えないようにする
	if (Hp.maxHp_ < Hp.hp_)
	{
		Hp.hp_ = Hp.maxHp_;
	}
	//	// 倍率を掛けない計算
	//	HPParam base;
	//
	//	//base.hp_ = this->Hp.hp_;
	//	// 最大 HP のアップグレード
	//	base.maxHP_ = config_->Count_.MAXHP_ + (int)param.HP.hpDelta.base;
	//	if (base.maxHP_ < 1u)
	//	{
	//		base.maxHP_ = 1u;
	//	}
	//
	//	this->Hp = base;
}

void PlayerParameter::ApplyAttack()
{
	// 足し算の計算
	AttackParam base;

	// 攻撃力
	base.slashPower_ = (config_->Power_.BASEPOWER_ + param.Attack.slashPowerDelta.base);
	// 攻撃範囲
	base.slashRange_ = (config_->Length_.WEAPONCOLLISIONRADIUS_ + param.Attack.slashRangeDelta.base);
	// 攻撃距離
	base.slashLength_ = (config_->Speed_.SLASH_ + param.Attack.slashLengthDelta.base);
	// 攻撃回数
	base.slashNum_ = config_->Count_.SLASHRELATIONBASE_ + (int)param.Attack.slashNumDelta.base;

	// 掛け算部分の計算
	AttackParam multi;

	// 攻撃力
	multi.slashPower_ = (0.01f * param.Attack.slashPowerDelta.percent);
	// 攻撃範囲
	multi.slashRange_ = (0.01f * param.Attack.slashRangeDelta.percent);
	// 攻撃距離
	multi.slashLength_ = (0.01f * param.Attack.slashLengthDelta.percent);
	// 攻撃回数(最低値)
	multi.slashNum_ = 1;

	this->Attack = base * multi;
}

void PlayerParameter::ApplySpeed()
{
	// 足し算の計算
	SpeedParam base;
	// 移動速度
	base.all_ = (param.Speed.allSpeedDelta.base);
	base.move_ = (config_->Speed_.MOVE_ + param.Speed.moveSpeedDelta.base);
	// 攻撃として設定済み
	base.slash_ = param.Speed.slashSpeedDelta.base;
	base.moment_ = (config_->Speed_.MOMENT_ + param.Speed.momentSpeedDelta.base);

	// 掛け算部分の計算
	SpeedParam multi;
	// 移動速度
	multi.all_ = (0.01f * param.Speed.allSpeedDelta.percent);
	multi.move_ = (0.01f * param.Speed.moveSpeedDelta.percent);
	multi.slash_ = (0.01f * param.Speed.slashSpeedDelta.percent);
	multi.moment_ = (0.01f * param.Speed.momentSpeedDelta.percent);

	this->Speed = base * multi;
	// 居合の距離を再設定
	Speed.slash_ += Attack.slashLength_;
}

void PlayerParameter::ApplyTime()
{
	// 足し算の計算
	TimeParam base;

	base.justTake_ = (config_->Time_.JUSTTAKETIME_ + param.Time.justTimeDelta.base);
	base.momentTime_ = (config_->Time_.MOMENTBASE_ + param.Time.momentTimeDelta.base);
	base.invincibleDamage_ = (config_->Time_.DAMAGEINVINCIBLE_ + param.Time.damageInvincibleTimeDelta.base);

	// 掛け算部分の計算
	TimeParam multi;

	multi.justTake_ = (0.01f * param.Time.justTimeDelta.percent);
	multi.momentTime_ = (0.01f * param.Time.momentTimeDelta.percent);
	multi.invincibleDamage_ = (0.01f * param.Time.damageInvincibleTimeDelta.percent);

	this->Time = base * multi;
}
