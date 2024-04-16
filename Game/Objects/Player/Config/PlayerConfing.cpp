#include "PlayerConfing.h"

void PlayerConfig::Initialize()
{
	// データの初期化
	// 読み込みとかもしたい
	InitSpeed();
	InitTime();
	InitLength();
	InitCount();
	InitParcentage();
	InitPower();
}

void PlayerConfig::InitSpeed()
{
	Speed_.MOVE_ = 15.0f;
	Speed_.SLASH_ = 100.0f;
	Speed_.MOMENT_ = 10.0f;
}

void PlayerConfig::InitTime()
{
	Time_.ROOTBASE_ = 0.5f;
	Time_.MOVEBASE_ = 0.0f;
	Time_.SLASHBASE_ = 0.3f;
	Time_.MOMENTBASE_ = 0.5f;
	Time_.DAMAGEBASE_ = 0.5f;

	Time_.MOMENTINCREMENT_ = 0.25f;
	Time_.JUSTTAKETIME_ = 0.1f;
	Time_.SLASHENDINVINCIBLETIME_ = 0.05f;
	Time_.JUSTINVINCIBLEADD_ = 0.1f;
	Time_.DAMAGEINVINCIBLE_ = 2.0f;
}

void PlayerConfig::InitLength()
{
	Length_.WEAPONCOLLISIONRADIUS_ = 1.5f;
	Length_.WEAPONPLUSCORRECTION_ = 4.0f;
	Length_.JUSTCOLLISIONRADIUS_ = 1.5f;
}

void PlayerConfig::InitCount()
{
	Count_.SLASHRELATIONBASE_ = 3u;
	Count_.BASEHP_ = 3u;
}

void PlayerConfig::InitParcentage()
{
	Parcent_.JUSTENABLE_ = 0.2f;
}

void PlayerConfig::InitPower()
{
	Power_.BASEPOWER_ = 10.0f;
}
