#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 小さな力
	/// </summary>
	class Skill_DamageInvincibleAdd : public ISkill
	{
	public:
		/// <summary>
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_DamageInvincibleAdd()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new InvincibleDamageDelta(2.0f));
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
			return "powerUp/PowerUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "Skill_DamageInvincibleAdd";
		}

	};
}