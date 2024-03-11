#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
	//enemys_.push_back(new NormalEnemy);
	//enemys_.push_back(new ShieldEnemy);
	//enemys_.push_back(new ArrowEnemy);
	for (IEnemy* enemy_ : enemys_)
	{
		enemy_->Initialize();
		enemy_->SetTarget(player_);
	}
}

void EnemyManager::Update()
{
	Flame++;
	if (Flame >= 120) {
		EnemySpown();
		Flame = 0;
	}

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

void EnemyManager::EnemySpown()
{
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	float number = 	distribution(randomEngine);
	std::uniform_real_distribution<float> distribution2(8.0f, 10.0f);
	float PtoE = distribution2(randomEngine);
	std::uniform_real_distribution<float> distribution3(0.0f, 1.0f);
	float divideX = distribution3(randomEngine);
	float divideZ = 1.0f - divideX;
	lwp::Vector3 pos = { PtoE * divideX , 0.5f , PtoE * divideZ };
	if (number <= 0.5f) {
		NormalEnemySpown(pos);
	}
	else if (number <= 0.8f) {
		ShieldEnemySpown(pos);
	}
	else {
		ArrowEnemySpown(pos);
	}
}

void EnemyManager::NormalEnemySpown(lwp::Vector3 pos)
{
	NormalEnemy* NEnemy = new NormalEnemy();
	NEnemy->Initialize();
	NEnemy->SetTarget(player_);
	NEnemy->SetPosition(pos);
	enemys_.push_back(NEnemy);
}

void EnemyManager::ShieldEnemySpown(lwp::Vector3 pos)
{
	ShieldEnemy* NEnemy = new ShieldEnemy();
	NEnemy->Initialize();
	NEnemy->SetTarget(player_);
	NEnemy->SetPosition(pos);
	enemys_.push_back(NEnemy);
}

void EnemyManager::ArrowEnemySpown(lwp::Vector3 pos)
{
	
	ArrowEnemy* NEnemy = new ArrowEnemy();
	NEnemy->Initialize();
	NEnemy->SetTarget(player_);
	NEnemy->SetPosition(pos);
	enemys_.push_back(NEnemy);
}
