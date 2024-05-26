#include "Burning.h"

#include <Game/Objects/Enemy/IEnemy/IEnemy.h>
#include <Game/Objects/Player/HitCheck/HitCheck.h>
#include <Game/Objects/Player/Player.h>
#define CALC_SCALAR 100

void L::Burning::Init()
{
}

void L::Burning::Update(HitEnemyTime* const het, Player* const player)
{
	if (!GetIsActive(player))
	{
		return;
	}
	// とりあえず 1 秒ごと
	if (int(het->hitTime * CALC_SCALAR) % kBurningInterval_ * CALC_SCALAR == 0)
	{
		// パラメータで % で上昇させてもいい
		het->target->DecreaseHP(player->parameter_.Attack.burningPower);
	}
}

void L::Burning::SlayEnemy(HitEnemyTime* const het, Player* const player)
{
	het;
	player;
}

bool L::Burning::GetIsActive(Player* const player)
{
	return player->parameter_.Flag.burningFlag;
}
