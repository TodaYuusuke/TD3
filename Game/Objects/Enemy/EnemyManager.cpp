#include "EnemyManager.h"
#include "../Player/Player.h"

void EnemyManager::Init()
{
	gameTimer_ = GameTimer::GetInstance();

	// ボスの発生フラグ
	isBossSpawn_ = false;
	// チュートリアルフラグ
	isTutorial_ = true;

	// static初期化
	IEnemy::InitStaticVariable();

	// チュートリアル用の敵を発生させる
	tutorialEnemy_ = new NormalEnemy();
	tutorialEnemy_->Initialize();
	tutorialEnemy_->SetCamera(followCamera_);
	tutorialEnemy_->SetTarget(player_);
	tutorialEnemy_->SetPosition(LWP::Math::Vector3{ 0,0.5f,10 });
	tutorialEnemy_->SetIsTutorial(true);
	tutorialEnemy_->SetHP(10);
	tutorialEnemy_->SetManager(exp_);
	enemys_.push_back(tutorialEnemy_);
}

void EnemyManager::Update()
{
	if (!isTutorial_)
	{
		currentFrame_++;

		// 通常敵の出現
		if (currentFrame_ >= kSpawnFrequency)
		{
			//ランダム生成用
			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());
			std::uniform_int_distribution<int> distribution(1, 3);
			int spawn = distribution(randomEngine);
			for (int It = 0; It < spawn; It++)
			{
				EnemySpawn();
			}
			currentFrame_ = 0;
		}
		// ボスキャラの出現
		BossSpawn();
	}
	else
	{
		// チュートリアルの敵を倒したらゲームスタート
		if (!tutorialEnemy_->GetIsTutorial())
		{
			isTutorial_ = false;
		}
	}

#ifdef DEMO
	DebugWindow();
#endif

	enemys_.remove_if([](IEnemy* enemy) {
		if (!enemy->GetIsActive())
		{
			delete enemy;
			return true;
		}
		return false;
		});
	for (IEnemy* enemy : enemys_)
	{
		enemy->Update();
		// 出現時のエフェクトのアニメーションが終了しているかをチェック
		enemy->CheckSpawnEffect();
	}
}

void EnemyManager::EnemySpawn()
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
	if (signX <= 0.5f)
	{
		signX = 1;
	}
	else
	{
		signX = -1;
	}
	if (signY <= 0.5f)
	{
		signY = 1;
	}
	else
	{
		signY = -1;
	}

	std::uniform_real_distribution<float> distribution2(8.0f, 10.0f);
	float PtoE = distribution2(randomEngine);

	lwp::Vector3 pos = { PtoE * divideX * signX * 5 , 0.5f , PtoE * divideZ * signY * 5 };
	if (number <= 0.5f)
	{
		NormalEnemySpawn(pos);
		//ArrowEnemySpawn(pos);
	}
	else if (number <= 0.8f)
	{
		ArrowEnemySpawn(pos);
	}
	else
	{
		ShieldEnemySpawn(pos);
		//NormalEnemySpawn(pos);
	}

}

void EnemyManager::BossSpawn()
{
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	float number = distribution(randomEngine);
	//方向を決める
	float divideX = distribution(randomEngine);
	float divideZ = 1.0f - divideX;
	float signY = 5.0f;

	std::uniform_real_distribution<float> distribution2(8.0f, 10.0f);
	float PtoE = distribution2(randomEngine);
	lwp::Vector3 pos = { PtoE * divideX , 1.5f , PtoE * divideZ * signY };

	// 1分
	// ダッシュボスを出現
	if (gameTimer_->GetCurrentSecond() == 01 && !isBossSpawn_)
	{
		DashBossSpawn(pos);
		//ArrowBossSpawn(pos);
		isBossSpawn_ = true;
	}
	// 2分
	// ホーミング弾を撃つボスを出現
	else if (gameTimer_->GetCurrentSecond() == 120 && isBossSpawn_)
	{
		ArrowBossSpawn(pos);
		isBossSpawn_ = false;
	}
}

void EnemyManager::NormalEnemySpawn(lwp::Vector3 pos)
{
	NormalEnemy* enemy = new NormalEnemy();
	enemy->Initialize();
	enemy->SetCamera(followCamera_);
	enemy->SetTarget(player_);
	enemy->SetPosition(pos);
	enemy->SetManager(exp_);
	enemy->SetEnemyHP(GetCurrentStage());
	enemys_.push_back(enemy);
}
void EnemyManager::ShieldEnemySpawn(lwp::Vector3 pos)
{
	ShieldEnemy* enemy = new ShieldEnemy();
	enemy->Initialize();
	enemy->SetCamera(followCamera_);
	enemy->SetTarget(player_);
	enemy->SetPosition(pos);
	enemy->SetManager(exp_);
	enemy->SetEnemyHP(GetCurrentStage());
	enemys_.push_back(enemy);
}
void EnemyManager::ArrowEnemySpawn(lwp::Vector3 pos)
{
	ArrowEnemy* enemy = new ArrowEnemy();
	enemy->Initialize();
	enemy->SetCamera(followCamera_);
	enemy->SetTarget(player_);
	enemy->SetPosition(pos);
	enemy->SetManager(exp_);
	enemy->SetEnemyHP(GetCurrentStage());
	enemys_.push_back(enemy);
}

void EnemyManager::DashBossSpawn(lwp::Vector3 pos)
{
	DashBoss* boss = new DashBoss();
	boss->Initialize();
	boss->SetCamera(followCamera_);
	boss->SetTarget(player_);
	boss->SetPosition(pos);
	boss->SetManager(exp_);
	boss->SetEnemyHP(GetCurrentStage());
	enemys_.push_back(boss);
}

void EnemyManager::ArrowBossSpawn(lwp::Vector3 pos)
{
	ArrowBoss* boss = new ArrowBoss();
	boss->Initialize();
	boss->SetCamera(followCamera_);
	boss->SetTarget(player_);
	boss->SetPosition(pos);
	boss->SetManager(exp_);
	boss->SetEnemyHP(GetCurrentStage());
	enemys_.push_back(boss);
}

void EnemyManager::JumpBossSpawn(lwp::Vector3 pos)
{
	JumpBoss* boss = new JumpBoss();
	boss->Initialize();
	boss->SetCamera(followCamera_);
	boss->SetTarget(player_);
	boss->SetPosition(pos);
	boss->SetManager(exp_);
	enemys_.push_back(boss);
}

void EnemyManager::DebugWindow()
{
	ImGui::Begin("EnemyManager");
	ImGui::InputInt("Spawn", &SpawnNum);
	if (ImGui::Button("Create"))
	{
		for (int it = 0; it < SpawnNum; it++)
		{
			EnemySpawn();
		}
	}
	ImGui::Text("%d", enemys_.size());

	ImGui::BeginChild(ImGui::GetID((void*)0));

	int index = 0;
	for (std::list<IEnemy*>::iterator itr = enemys_.begin(); itr != enemys_.end(); ++itr)
	{
		if (ImGui::TreeNode(("Enemy" + std::to_string(index)).c_str()))
		{
#ifdef DEMO
			(*itr)->DebugPrint();
#endif

			ImGui::TreePop();
			ImGui::Separator();
		}
		index++;
	}

	ImGui::EndChild();

	ImGui::End();
}

int EnemyManager::GetCurrentStage()
{
	return gameTimer_->GetCurrentSecond() / 60;
}
