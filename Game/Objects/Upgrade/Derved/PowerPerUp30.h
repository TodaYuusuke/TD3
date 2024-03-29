#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な攻撃力アップ！（定数）
	/// </summary>
	class PowerPerUp30 : public IUpgrade
	{

		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->power.percent += 30.0f;
		}

		std::string GetTexturePass() override
		{
			return "powerUp/PowerUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "PowerUpPer";
		}
	};
}