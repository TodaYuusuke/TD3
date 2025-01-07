#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な移動速度アップ！（定数）
	/// </summary>
	class AllSpeedDelta : public IUpgrade
	{
	private:
		// 変化させる値
		Paramete var = 0.0f;
	public:
		/// <summary>
		/// 純粋な移動速度アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		AllSpeedDelta(float b = 0.0f, float per = 0.0f) : var(b, per) {};
		AllSpeedDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Speed.allSpeedDelta += var;
		}

		void GetUpgradeName() override
		{
			ImGui::Text("AllSpeed : %.2f, %.2f", var.base, var.percent);
		}
	};
}