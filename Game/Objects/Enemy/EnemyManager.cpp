#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
	ArrowBossSpown(LWP::Math::Vector3{ 0,0,10 });
}

void EnemyManager::Update()
{
	currentFrame_++;
	if (currentFrame_ >= kSpownFrequency) {
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		std::uniform_int_distribution<int> distribution(1, 3);
		int Spown = distribution(randomEngine);
		for (int It = 0; It < Spown; It++) {
			EnemySpown();
		}
		currentFrame_ = 0;
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
	float number = distribution(randomEngine);
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
		//NormalEnemySpown(pos);
		// ボスの発生
		//DashBossSpown(pos);
		//ArrowBossSpown(pos);
	}
	else if (number <= 0.8f) {
		//ShieldEnemySpown(pos);
	}
	else {
		//ArrowEnemySpown(pos);
	}

}
void EnemyManager::NormalEnemySpown(lwp::Vector3 pos)
{
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Initialize();
	enemy->SetCamera(followCamera_);
	enemy->SetTarget(player_);
	enemy->SetPosition(pos);
	enemys_.push_back(enemy);
}
void EnemyManager::ShieldEnemySpown(lwp::Vector3 pos)
{
	ShieldEnemy* enemy = new ShieldEnemy();
	enemy->Initialize();
	enemy->SetCamera(followCamera_);
	enemy->SetTarget(player_);
	enemy->SetPosition(pos);
	enemys_.push_back(enemy);
}
void EnemyManager::ArrowEnemySpown(lwp::Vector3 pos)
{
	ArrowEnemy* enemy = new ArrowEnemy();
	enemy->Initialize();
	enemy->SetCamera(followCamera_);
	enemy->SetTarget(player_);
	enemy->SetPosition(pos);
	enemys_.push_back(enemy);
}

void EnemyManager::DashBossSpown(lwp::Vector3 pos) {
	DashBoss* boss = new DashBoss();
	boss->Initialize();
	boss->SetCamera(followCamera_);
	boss->SetTarget(player_);
	boss->SetPosition(pos);
	enemys_.push_back(boss);
}

void EnemyManager::ArrowBossSpown(lwp::Vector3 pos) {
	ArrowBoss* boss = new ArrowBoss();
	boss->Initialize();
	boss->SetCamera(followCamera_);
	boss->SetTarget(player_);
	boss->SetPosition(pos);
	enemys_.push_back(boss);
}