#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 経験値取得範囲増加
	/// </summary>
	class Skill_RadiusLevelUp : public ISkill
	{
	public:
		/// <summary>
		/// 経験値取得範囲増加
		/// </summary>
		Skill_RadiusLevelUp()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new RadiusLevelDelta(2.0f, 0.0f));
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
			return "Skill_RadiusLevelUp";
		}

	};
}