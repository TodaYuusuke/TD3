#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 追撃のダメージ量を変更する
	/// </summary>
	class PursuitPowerDelta : public IUpgrade
	{
	private:
		// 変化させる値
		Paramete var;
	public:
		/// <summary>
		/// 追撃
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		PursuitPowerDelta(float b = 0.0f, float per = 0.0f) : var(b, per) {};
		PursuitPowerDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Attack.pursuitDelta.base += var.base;
			para->Attack.pursuitDelta.percent += var.percent;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("PursuitPower : % .2f, % .2f", var.base, var.percent);
			//return std::format("Power : %.2f, %.2f", var.base, var.percent);
		}
	};
}