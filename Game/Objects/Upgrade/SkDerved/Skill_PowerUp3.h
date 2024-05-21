#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 強力な力
	/// </summary>
	class Skill_PowerUp3 : public ISkill
	{
	public:
		/// <summary>
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_PowerUp3()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new PowerPerDelta(40.0f));
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
			return "katana128.png";
		}

		std::string GetUpgradeName() override
		{
			return "Skill_PowerUP3";
		}

	};
}