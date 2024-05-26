#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 継続ダメージのダメージ量を変更する
	/// </summary>
	class BurningPowerDelta : public IUpgrade
	{
	private:
		// 変化させる値
		Paramete var;
	public:
		/// <summary>
		/// 継続ダメージ
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		BurningPowerDelta(float b = 0.0f, float per = 0.0f) : var(b, per) {};
		BurningPowerDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Attack.burningDelta.base += var.base;
			para->Attack.burningDelta.percent += var.percent;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("BuringPower : % .2f, % .2f", var.base, var.percent);
			//return std::format("Power : %.2f, %.2f", var.base, var.percent);
		}
	};
}