#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 敵に継続ダメージを与える
	/// 燃焼でも毒でも何でもいい
	/// </summary>
	class Skill_BurningFlag : public ISkill
	{
	private:
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Skill_BurningFlag() = default;



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			// フラグを true にして使えるようにする
			para->Flag.burningFlag = true;
		}

		std::string GetTexturePass() override
		{
			return "Skill/Burning.png";
		}

		std::string GetUpgradeName() override
		{
			return "BurningFlag";
		}
	};
}