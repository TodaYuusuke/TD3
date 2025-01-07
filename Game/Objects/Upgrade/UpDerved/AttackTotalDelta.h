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
		int var = 0;
	public:
		/// <summary>
		/// 純粋な攻撃回数アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		AttackTotalDelta(int v = 1) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Attack.slashNumDelta.base += var;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("AttackTotal : %d", var);
		}
	};
}