#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 経験値取得範囲増加
	/// </summary>
	class RadiusLevelDelta : public IUpgrade
	{
	private:
		// 変化させる値
		Paramete var;
	public:
		/// <summary>
		/// 半径を増減させる
		/// </summary>
		RadiusLevelDelta(float b = 0.0f, float per = 100.0f) : var(b, per) {};
		RadiusLevelDelta(Paramete v) : var(v) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->Other.radiusLevel.base += var.base;
			para->Other.radiusLevel.percent += var.percent;
		}

	};
}