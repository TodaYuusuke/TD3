#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な攻撃倍率アップ！（定数）
	/// </summary>
	class PowerPerDelta : public IUpgrade
	{
	private:
		// 変化させる値
		float var = 0.0f;
	public:
		/// <summary>
		/// 純粋な攻撃倍率アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		PowerPerDelta(float v = 10.0f) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Attack.slashPowerDelta.percent += var;
		}

		std::string GetUpgradeName() override
		{
			return "PowerPerDelta : " + std::to_string(var);
		}
	};
}