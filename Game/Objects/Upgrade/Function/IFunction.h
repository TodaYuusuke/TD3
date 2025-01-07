#pragma once

#include <Adapter.h>

class Player;

namespace L
{
	// 前方宣言
	struct HitEnemyTime;

	/// <summary>
	/// 攻撃ヒット後の計算をまとめるための基底クラス
	/// </summary>
	class IFunction
	{
	public:
		IFunction() = default;
		virtual ~IFunction() = default;
	public:
		// 初期化
		virtual void Init() = 0;
		// 更新
		virtual void Update(HitEnemyTime* const het, Player* const player) = 0;
		// 敵が死んだときの判定
		virtual void SlayEnemy(HitEnemyTime* const het, Player* const player) { het; player; };
	protected:
		// 機能が有効中かどうか
		// 何か更新だけはするかもしれないから更新関数内で判断
		virtual bool GetIsActive() { return false; }
	};
}