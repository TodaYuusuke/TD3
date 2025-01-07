#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な攻撃力アップ！（定数）
	/// </summary>
	class PowerDelta : public IUpgrade
	{
	private:
		// 変化させる値
		Paramete var;
	public:
		/// <summary>
		/// 純粋な攻撃力アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		PowerDelta(float b = 0.0f, float per = 0.0f) : var(b, per) {};
		PowerDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Attack.slashPowerDelta.base += var.base;
			para->Attack.slashPowerDelta.percent += var.percent;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("Power : % .2f, % .2f", var.base, var.percent);
			//return std::format("Power : %.2f, %.2f", var.base, var.percent);
		}
	};
}