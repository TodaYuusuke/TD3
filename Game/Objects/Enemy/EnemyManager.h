#pragma once
#include "IEnemy/IEnemy.h"
#include "NormalEnemy/NormalEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "ArrowEnemy/ArrowEnemy.h"

class Player;
class EnemyManager
{
public:
	void Init();
	void Update();

	void SetPlayer(Player* player) { player_ = player; }

private:
	std::list<std::unique_ptr<IEnemy>>enemys_;
	Player* player_;
};