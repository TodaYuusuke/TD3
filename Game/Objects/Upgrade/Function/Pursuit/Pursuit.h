#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "Game/Objects/Enemy/ArrowEnemy/Arrow/Arrow.h"
class Pursuit {
public:
	Pursuit(Player* const player);

	void Init();
	bool Execution();
	void AddEnemy(IEnemy* enemy) {
		enemys_.push_back(enemy);
	}
private:

	// ボスも含めたすべての敵
	std::vector<IEnemy*> enemys_;
	// 攻撃力
	int* damage_ = nullptr;
	const int kInterval = 60;
	int interval = 30;
	std::vector<LWP::Primitive::Mesh> models_;
};