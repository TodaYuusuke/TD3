#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な攻撃回数アップ！（定数）
	/// </summary>
	class AttackTotalDelta : public IUpgrade
	{
	private:
		// 変化させる値
		float var = 0.0f;
	public:
		/// <summary>
		/// 純粋な攻撃回数アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		AttackTotalDelta(float v = 1.0f) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Attack.slashNumDelta.base += var;
		}

		std::string GetTexturePass() override
		{
			return "powerUp/PowerUp.png";
		}

		std::string GetUpgradeName() override
		{
			return "AttackTotalDelta : " + std::to_string(var);
		}
	};
}