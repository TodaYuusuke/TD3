#pragma once
#include <Adapter.h>

/// <summary>
/// ほぼすべてのアップグレードをこの型にしてもいいと思う
/// </summary>
struct Paramete
{
	// 基準(定数)
	float base = 0.0f;
	// 補正(％)
	float percent = 100.0f;
};

struct UpgradeHP
{
	// 体力 : 倍率は使わない
	Paramete hpDelta;


};

struct UpgradeAttack
{
	// 攻撃力
	Paramete slashPowerDelta;
	// 攻撃範囲
	Paramete slashRangeDelta;
	// 攻撃距離
	Paramete slashLengthDelta;
	// 攻撃回数 : 倍率は使わない
	Paramete slashNumDelta;

};

struct UpgradeSpeed
{
	// すべての移動速度
	Paramete allSpeedDelta;
	// 歩きの移動速度
	Paramete moveSpeedDelta;
	// 抜刀の移動速度
	Paramete slashSpeedDelta;
	// 後隙の移動速度
	Paramete momentSpeedDelta;

};

struct UpgradeTime
{
	// ジャスト判定の時間
	Paramete justTimeDelta;
	// 後隙の時間
	Paramete momentTimeDelta;
	// 被弾時の無敵時間
	Paramete damageInvincibleTimeDelta;

};

/// <summary>
/// 新機能のフラグを作る
/// </summary>
struct UpgradeFlag
{
	// 攻撃後追撃
	bool pursuitFlag = false;
	// バリア
	bool eXLifeFlag = false;
	// 吹き飛ばし
	bool BlowOffFlag = false;
	// 敵を複数倒すと HP 回復
	bool penetrationFlag = false;
};

/// <summary>
/// アップグレードによって変化する値と倍率を司る
/// </summary>
struct UpgradeParameter
{
	// HP のみ
	UpgradeHP HP;
	// 攻撃関係
	UpgradeAttack Attack;
	// 移動速度のみ
	UpgradeSpeed Speed;
	// 時間のみ
	UpgradeTime Time;
	// 新機能のフラグ
	UpgradeFlag Flag;
};

namespace L
{
	/// <summary>
	/// アップグレード基底クラス（新しいアップグレードはこれを継承してつくること）
	/// </summary>
	class IUpgrade
	{
	public:	// ** パブリックなメンバ変数 ** //

		/// <summary>
		/// アップグレードを適応する関数
		/// </summary>
		/// <param name="p"></param>
		virtual void Apply(UpgradeParameter* para) = 0;

		/// <summary>
		/// デバッグ表示関数
		/// </summary>
		virtual std::string GetUpgradeName() { return "Upgrade"; }

	protected: // ** 派生先用のメンバ変数 ** //

	};
}
