#pragma once
#include "IEnemy/IEnemy.h"
#include "NormalEnemy/NormalEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "ArrowEnemy/ArrowEnemy.h"
#include "Boss/DashBoss.h"
#include "Game/Objects/Experience/ExpManager.h"

#include <random>
#include <numbers>

class Player;
class EnemyManager
{
public:
	/// 
	/// Default Method
	/// 

	~EnemyManager()
	{
		for (IEnemy* e : enemys_)
		{
			delete e;
		}
	}
	void Init();
	void Update();

	/// 
	/// User Method
	/// 

#pragma region 敵の発生
	// 敵の発生場所をランダムで決める
	void EnemySpown();
	// 通常
	void NormalEnemySpown(lwp::Vector3 pos);
	// 盾
	void ShieldEnemySpown(lwp::Vector3 pos);
	// 弓
	void ArrowEnemySpown(lwp::Vector3 pos);
	// ダッシュするボス
	void DashBossSpown(lwp::Vector3 pos);
#pragma endregion

	/// Getter

	/// Setter
	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }
	void SetExpManager(ExpManager* p) { exp_ = p; }

private:// 定数
	// 敵発生頻度
	const int kSpownFrequency = 120;

private:
	// ボスも含めたすべての敵
	std::list<IEnemy*> enemys_;
	// 自機
	Player* player_;

	// 経験値マネージャー
	ExpManager* exp_;

	// 現在のフレーム
	int currentFrame_ = 0;

	int SpawnNum = 0;

private: //*** プライベート関数 ***//

	void DebugWindow();

};