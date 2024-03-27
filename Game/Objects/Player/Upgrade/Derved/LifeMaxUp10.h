#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な攻撃回数アップ！（定数）
	/// </summary>
	class LifeMaxUp10 : public IUpgrade
	{

		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->lifeMax.base += 10;
		}

		std::string GetTexturePass() override
		{
			return "powerUp/PowerUp.png";
		}
	};
}