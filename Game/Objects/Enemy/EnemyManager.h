#pragma once
#include "IEnemy/IEnemy.h"
#include "NormalEnemy/NormalEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "ArrowEnemy/ArrowEnemy.h"
#include <random>
#include <numbers>

class Player;
class EnemyManager
{
public:
	/// 
	/// Default Method
	/// 
	
	void Init();
	void Update();

	/// 
	/// User Method
	/// 

	void EnemySpown();
	void NormalEnemySpown(lwp::Vector3 pos);
	void ShieldEnemySpown(lwp::Vector3 pos);
	void ArrowEnemySpown(lwp::Vector3 pos);

	/// Getter

	/// Setter
	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }

private:
	std::list<IEnemy*> enemys_;
	Player* player_;

	int Flame = 0;
};