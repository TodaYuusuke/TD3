#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 被弾時の無敵時間を 2 秒追加する
	/// </summary>
	class Skill_DamageInvincibleAdd : public ISkill
	{
	public:
		/// <summary>
		/// 被弾時の無敵時間を 2 秒追加する
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_DamageInvincibleAdd()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new InvincibleDamageDelta(2.0f, 0.0f));
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
			return "Skill/InvincibleUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "DamageInvincibleAdd";
		}

	};
}