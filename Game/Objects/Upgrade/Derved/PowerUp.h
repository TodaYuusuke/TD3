#pragma once
#include "../IUpgrade.h"

namespace L {
	/// <summary>
	/// 純粋な攻撃力アップ！（定数）
	/// </summary>
	class PowerUp : public IUpgrade {

		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override {
			para->slashPowerDelta.base += 10;
		}

		std::string GetTexturePass() override {
			return "powerUp/PowerUp.png";
		}

		std::string GetUpgradeName() override {
			return "PowerUp";
		}
	};
}