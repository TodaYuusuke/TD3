#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
	enemys_.push_back(new NormalEnemy);
	enemys_.push_back(new ShieldEnemy);
	enemys_.push_back(new ArrowEnemy);
	for (IEnemy* enemy_ : enemys_)
	{
		enemy_->Initialize();
		enemy_->SetTarget(player_);
	}
}

void EnemyManager::Update()
{
	// 消しながら更新
	for (std::list<IEnemy*>::iterator itr = enemys_.begin(); itr != enemys_.end();)
	{
		// 消えているなら
		if (!(*itr)->GetIsActive())
		{
			delete* itr;
			itr = enemys_.erase(itr);
			continue;
		}
		// 更新
		(*itr)->Update();

		// 次へ
		++itr;
	}
}
