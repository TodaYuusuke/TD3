#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 後隙時間変更
	/// </summary>
	class MomentTimeDelta : public IUpgrade
	{
	private:
		// 変化させる値
		Paramete var = 0.0f;
	public:
		/// <summary>
		/// 後隙時間変更
		/// <para>ここで値を定義する</para>
		/// </summary>
		/// <param name="v">100.0f ~ 0.0f の ％</param>
		MomentTimeDelta(float b = 0.0f, float per = 0.0f) : var(b, per) {};
		MomentTimeDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Time.momentTimeDelta += var;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("MomentTime : %.2f, %.2f", var.base, var.percent);
		}
	};
}