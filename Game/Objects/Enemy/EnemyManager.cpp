#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
}

void EnemyManager::Update()
{
	//Flame++;
	if (Flame >= 120) {
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		std::uniform_int_distribution<int> distribution(1, 3);
		int Spown = distribution(randomEngine);
		for (int It = 0; It < Spown; It++) {
			EnemySpown();
		}
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
	//方向を決める
	float divideX = distribution(randomEngine);
	float divideZ = 1.0f - divideX;
	float signX = distribution(randomEngine);
	float signY = distribution(randomEngine);
		if (signX <= 0.5f) {
			signX = 1;
		}
		else {
			signX = -1;
		}
		if (signY <= 0.5f) {
			signY = 1;
		}
		else {
			signY = -1;
		}

	std::uniform_real_distribution<float> distribution2(8.0f, 10.0f);
	float PtoE = distribution2(randomEngine);

	lwp::Vector3 pos = { PtoE * divideX * signX , 0.5f , PtoE * divideZ * signY };
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
