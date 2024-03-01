#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	enemys_.push_back(std::make_unique<Enemy>());
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->Initialize();
	}
}

void EnemyManager::Update()
{
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->Update();
	}
}
