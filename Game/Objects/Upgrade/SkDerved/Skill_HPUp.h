#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// HPを 2 増加させる
	/// </summary>
	class Skill_HPUp : public ISkill
	{
	public:
		/// <summary>
		/// HPを 2 増加させる
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_HPUp()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new LifeMaxDelta(2));
		}

		///// <summary>
		///// アップグレード数を返す
		///// <para>オーバーロードする</para>
		///// </summary>
		///// <returns>配列数</returns>
		//size_t GetUpgradeNum() override
		//{
		//	return 2;
		//}


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
			return "Skill_HPUp";
		}

	};
}