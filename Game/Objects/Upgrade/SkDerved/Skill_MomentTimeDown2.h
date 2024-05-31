#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 後隙を 30% 減少させる
	/// </summary>
	class Skill_MomentTimeDown2 : public ISkill
	{
	public:
		/// <summary>
		/// 後隙を 30% 減少させる
		/// <para>ここでアップグレードを定義する</para>
		/// </summary>
		Skill_MomentTimeDown2()
		{
			attackUpgrades_.clear();
			attackUpgrades_.push_back(new MomentTimeDelta(0.0f, -30.0f));
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
			return "MomentTimeDown2";
		}

	};
}