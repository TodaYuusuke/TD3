#pragma once

#include "../IFunction.h"

namespace L
{
	// 敵を一定数倒したときにプレイヤーの HP を回復させる
	class Penetration :public IFunction
	{
	private:
		// 燃えるダメージ
		const float destroyEnemyNum_ = 3.0f;
		// ダメージを与える間隔
		const int kBurningInterval_ = 1;

		// 倒した敵の数を参照する
		// 今何体倒しているのか
		uint32_t* pDestroyEnemyNum_ = nullptr;
		uint32_t* pPostDestroyEnemyNum_ = nullptr;


	public:
		Penetration(uint32_t* const p1, uint32_t* const p2) : pDestroyEnemyNum_(p1), pPostDestroyEnemyNum_(p2) {}

		// 初期化
		void Init() override;
		// 更新
		void Update(HitEnemyTime* const het, Player* const player) override;
		// 敵が死んだ時の判定
		void SlayEnemy(HitEnemyTime* const het, Player* const player) override;
	private:
		bool GetIsActive() override;

	};
}