#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// そこそこの力
	/// </summary>
	class Skill_PowerUp2 : public ISkill
	{
	public:
		/// <summary>
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_PowerUp2()
		{
			upgrades_.clear();
			upgrades_.push_back(new PowerDelta(5.0f));
			upgrades_.push_back(new PowerPerDelta(20.0f));
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
			return "Skill_PowerUp2";
		}

	};
}