#pragma once
#include "IEnemy/IEnemy.h"
#include "NormalEnemy/NormalEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "ArrowEnemy/ArrowEnemy.h"

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

	/// Getter

	/// Setter
	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }

private:
	std::list<IEnemy*>enemys_;
	Player* player_;
};