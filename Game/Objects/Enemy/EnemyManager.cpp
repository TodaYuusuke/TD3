#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
	//enemys_.push_back(std::make_unique<NormalEnemy>());
	//enemys_.push_back(std::make_unique<ShieldEnemy>());
	enemys_.push_back(new ArrowEnemy());
	
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
