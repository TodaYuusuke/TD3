#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 攻撃の長さをアップさせる
	/// </summary>
	class Skill_AttackLengthUp : public ISkill
	{
	public:
		/// <summary>
		/// 攻撃の長さをアップさせる
		/// 20% アップ
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_AttackLengthUp()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new AttackLengthDelta(0.0f, 20.0f));
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
			return "Skill/AttackLengthUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "Skill_AttacklengthUp";
		}

	};
}