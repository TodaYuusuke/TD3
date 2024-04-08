#pragma once
#include <Adapter.h>
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "Game/Objects/Enemy/ArrowEnemy/Arrow/Arrow.h"
class Pursuit {
public:
	void Init();
	bool Execution();
	void AddEnemy(ICollider* enemy) {
		enemys_.push_back(enemy);
	}
private:
	void CreateCollider();

	// ボスも含めたすべての敵
	std::vector<ICollider*> enemys_;
	// 攻撃力
	int damage_ = 1;
	const int kInterval = 120;
	int interval = 120;
	std::vector<LWP::Primitive::Mesh> models_;
	//LWP::Primitive::Mesh models_;
	std::vector<LWP::Object::Collider::AABB> aabb_;
};