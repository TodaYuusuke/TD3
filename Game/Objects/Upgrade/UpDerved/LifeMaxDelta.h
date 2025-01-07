#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な最大HPアップ！（定数）
	/// </summary>
	class LifeMaxDelta : public IUpgrade
	{
	private:
		// 変化させる値
		int var = 0;
	public:
		/// <summary>
		/// 純粋な最大HPアップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		LifeMaxDelta(int v = 1) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->HP.hpDelta.base += var;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("LifeMax : %d", var);
		}
	};
}