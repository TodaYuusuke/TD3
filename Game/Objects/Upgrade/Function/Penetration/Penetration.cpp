#include "Penetration.h"

#include <Game/Objects/Player/Player.h>

void L::Penetration::Init()
{
}

void L::Penetration::Update(HitEnemyTime* const het, Player* const player)
{
}

void L::Penetration::SlayEnemy(HitEnemyTime* const het, Player* const player)
{
	// ここでアップグレードのフラグを使いたい
	// HP 回復出来ますか？
	if (player->parameter_.Flag.penetrationFlag)
	{
		// 一定以上かつ前のフレームと一緒ではない
		// 一定値以上になった瞬間をコールバックさせたい
		if (*pDestroyEnemyNum_ != *pPostDestroyEnemyNum_ &&
			*pDestroyEnemyNum_ % player->config_.Count_.UPGRADEPENETORATIONNUM_ == 0)
		{
			// HP 回復を実行できる状態にする
			player->parameter_.Flag.isActiveIncreaseHP = true;
		}
	}
}

bool L::Penetration::GetIsActive()
{
    return false;
}
