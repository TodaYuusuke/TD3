#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 無敵時間アップ（定数）
	/// </summary>
	class InvincibleDamageDelta : public IUpgrade
	{
	private:
		// 変化させる値
		float var = 0.0f;
	public:
		/// <summary>
		/// 無敵時間アップ（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		InvincibleDamageDelta(float v = 1.0f) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Time.damageInvincibleTimeDelta.base += var;
		}

		std::string GetUpgradeName() override
		{
			return "PowerDelta : " + std::to_string(var);
		}
	};
}