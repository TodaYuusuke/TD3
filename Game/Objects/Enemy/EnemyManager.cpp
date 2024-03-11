#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
	enemys_.push_back(new NormalEnemy());
	//enemys_.push_back(new ShieldEnemy());
	//enemys_.push_back(new ArrowEnemy());
	
	for (IEnemy* enemy : enemys_) {
		enemy->Init();
		enemy->SetTarget(player_);
	}
}

void EnemyManager::Update()
{
	for (IEnemy* enemy : enemys_) {
		enemy->Update();
		//enemy->Move({0.0f,1.0f,0.0f});
	}
}
