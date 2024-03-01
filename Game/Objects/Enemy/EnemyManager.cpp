#include "EnemyManager.h"

void EnemyManager::Init()
{
	enemys_.push_back(std::make_unique<NormalEnemy>());
	enemys_.push_back(std::make_unique<ShieldEnemy>());
	enemys_.push_back(std::make_unique<ArrowEnemy>());
	for (std::unique_ptr<IEnemy>& enemy_ : enemys_) {
		enemy_->Init();
	}
}

void EnemyManager::Update()
{

	for (std::unique_ptr<IEnemy>& enemy_ : enemys_) {
		enemy_->Update();
		enemy_->Move({0.0f,0.01f,0.0f});
	}
}
