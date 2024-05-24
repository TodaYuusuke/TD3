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
		Paramete var = 0.0f;
	public:
		/// <summary>
		/// 無敵時間アップ（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		InvincibleDamageDelta(float b = 0.0f, float per = 0.0f) : var(b, per) {};
		InvincibleDamageDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Time.damageInvincibleTimeDelta += var;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("InvincibleDamage : %.2f, %.2f", var.base, var.percent);
		}
	};
}