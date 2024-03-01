#pragma once
#include "Enemy.h"

class EnemyManager
{
public:
	void Initialize();
	void Update();

private:
	std::list<std::unique_ptr<Enemy>>enemys_;
};