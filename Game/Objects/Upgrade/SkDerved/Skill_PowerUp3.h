#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 強力な力
	/// </summary>
	class Skill_PowerUP3 : public ISkill
	{
	public:
		/// <summary>
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_PowerUP3()
		{
			upgrades_.clear();
			upgrades_.push_back(new PowerPerDelta(40.0f));
		}

		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			for (IUpgrade* up : upgrades_)
			{
				up->Apply(para);
			}
		}

		std::string GetTexturePass() override
		{
			return "powerUp/PowerUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "Skill_PowerUP3";
		}

	};
}