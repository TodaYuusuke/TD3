#pragma once

#include <Adapter.h>

#include <list>
#include <map>

// 前方宣言
class IEnemy;
class Player;

namespace L
{
	// 攻撃がヒットしてどの程度経ったかまでの情報
	struct HitEnemyTime
	{
		// 当たった敵
		IEnemy* target = nullptr;
		// 当たってからの時間
		float hitTime = 0.0f;

		HitEnemyTime(IEnemy* e) : target(e), hitTime(0.0f) {}
	};

	/// <summary>
	/// プレイヤーに持たせる
	/// <para>敵を登録したり、更新させる</para>
	/// </summary>
	class HitCheck
	{
	private:

		// 当たった後更新する時間
		const float kMaxTime = 10.0f;

		// 攻撃が当たった敵を格納
		std::list<HitEnemyTime*> hitList_;

		// 実際の敵の数を格納
		std::map<uint32_t, int> hitEnemys_;

		// 今何体倒しているのか
		uint32_t destroyEnemyNum_ = 0u;
		uint32_t postDestroyEnemyNum_ = 0u;

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update(Player* const player);

		/// <summary>
		/// 攻撃に当たった敵を登録
		/// </summary>
		void AddHitEnemy(IEnemy* enemy);

	private:

		/// <summary>
		/// 削除するかどうかを確認する
		/// </summary>
		bool CheckDeleteTarget(HitEnemyTime* het);

		/// <summary>
		/// フラグを参照して更新する
		/// </summary>
		void CheckFlags(HitEnemyTime* het, Player* const player);

		/// <summary>
		/// HP 回復できるか判断
		/// </summary>
		void CheckPenetrate(Player* const player);

	};
}