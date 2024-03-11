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
	if (Flame >= 200) {
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

	if (number <= 0.5f) {
		NormalEnemySpown(randomEngine);
	}
	else if (number <= 0.8f) {
		ShieldEnemySpown(randomEngine);
	}
	else {
		ArrowEnemySpown(randomEngine);
	}
}

void EnemyManager::NormalEnemySpown(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(5.0f, 10.0f);
	lwp::Vector3 pos = { distribution(randomEngine) , 0.0f , distribution(randomEngine) };
	NormalEnemy* NEnemy = new NormalEnemy();
	NEnemy->Init();
	NEnemy->SetPosition(pos);
	NEnemy->SetTarget(player_);
	enemys_.push_back(NEnemy);
}

void EnemyManager::ShieldEnemySpown(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(5.0f, 10.0f);
	lwp::Vector3 pos = { distribution(randomEngine) , 0.0f , distribution(randomEngine) };
	ShieldEnemy* NEnemy = new ShieldEnemy();
	NEnemy->Init();
	NEnemy->SetPosition(pos);
	NEnemy->SetTarget(player_);
	enemys_.push_back(NEnemy);
}

void EnemyManager::ArrowEnemySpown(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(5.0f, 10.0f);
	lwp::Vector3 pos = { distribution(randomEngine) , 0.0f , distribution(randomEngine) };
	ArrowEnemy* NEnemy = new ArrowEnemy();
	NEnemy->Init();
	NEnemy->SetPosition(pos);
	NEnemy->SetTarget(player_);
	enemys_.push_back(NEnemy);
}
