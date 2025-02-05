#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 攻撃の横幅をアップさせる
	/// </summary>
	class Skill_AttackRangeUp : public ISkill
	{
	public:
		/// <summary>
		/// 攻撃の横幅をアップさせる
		/// 1.0f 上昇 10% 上昇
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_AttackRangeUp()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new AttackRangeDelta(1.0f, 10.0f));
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
			return "Skill/AttackRangeUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "AttackRangeUp";
		}

	};
}