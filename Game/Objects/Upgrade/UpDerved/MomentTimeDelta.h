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
		float var = 0.0f;
	public:
		/// <summary>
		/// 後隙時間変更
		/// <para>ここで値を定義する</para>
		/// </summary>
		/// <param name="v">100.0f ~ 0.0f の ％</param>
		MomentTimeDelta(float v = 100.0f) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Time.momentTimeDelta.percent += var;
		}

		std::string GetUpgradeName() override
		{
			return "MomentTime : " + std::to_string(var);
		}
	};
}