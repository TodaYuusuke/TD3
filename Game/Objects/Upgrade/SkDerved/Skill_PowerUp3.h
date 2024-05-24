#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 攻撃力を 5 と 20% 上昇させる
	/// 後隙を 10% 増やす
	/// </summary>
	class Skill_PowerUp3 : public ISkill
	{
	public:
		/// <summary>
		/// 攻撃力を 40% 上昇させる
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_PowerUp3()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new PowerDelta(5.0f, 20.0f));
			attackUpgrades_.push_back(new MomentTimeDelta(0.0f, 10.0f));
		}

		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			for (IUpgrade* up : attackUpgrades_)
			{
				up->Apply(para);
			}
		}

		std::string GetTexturePass() override
		{
			return "Skill/white.png";
		}

		std::string GetUpgradeName() override
		{
			return "PowerUp3";
		}

	};
}