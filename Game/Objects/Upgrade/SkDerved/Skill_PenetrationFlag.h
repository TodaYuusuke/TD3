#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 敵を15体数倒したときに回復するようになる
	/// </summary>
	class Skill_PenetrationFlag : public ISkill
	{
	private:
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Skill_PenetrationFlag() = default;



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			// フラグを true にして使えるようにする
			para->Flag.penetrationFlag = true;
		}

		std::string GetTexturePass() override
		{
			return "Skill/Penetration.png";
		}

		std::string GetUpgradeName() override
		{
			return "PenetrationFlag";
		}
	};
}