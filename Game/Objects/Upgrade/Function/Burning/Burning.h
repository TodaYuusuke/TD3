#pragma once

#include "../IFunction.h"

namespace L
{
	// 敵を一定時間燃やしてダメージを与える
	class Burning :public IFunction
	{
	private:
		// 燃えるダメージ
		//const float kBurningDamage_ = 1.0f;
		// ダメージを与える間隔
		const int kBurningInterval_ = 10;
	public:
		// 初期化
		void Init() override;
		// 更新
		void Update(HitEnemyTime* const het, Player* const player) override;
		// 敵が死んだ時の判定
		void SlayEnemy(HitEnemyTime* const het, Player* const player) override;
	private:
		bool GetIsActive(Player* const player);

	};
}