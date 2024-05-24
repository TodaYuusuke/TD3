#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 攻撃力を 10 追加し更に 20% 上昇させる
	/// </summary>
	class Skill_PowerUp2 : public ISkill
	{
	public:
		/// <summary>
		/// 攻撃力を 5 追加し更に 10% 上昇させる
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_PowerUp2()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new PowerDelta(5.0f, 10.0f));
			attackUpgrades_.push_back(new AttackLengthDelta(0.0f, 10.0f));
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
			return "PowerUp2";
		}

	};
}