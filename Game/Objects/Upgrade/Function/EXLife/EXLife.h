#pragma once
#include <Adapter.h>
class Player;
class EXLife
{
public:
	void Init(Player* Player);
	bool Execution();

	void DecreaseHP();

private:
	Player* player_;
	float time_ = 0;
	uint32_t EXLife_ = 1;
	
	const float kHealInterval = 60.0f;
	lwp::Sprite sprite;

	// 元の位置
	lwp::Vector3 basePosition{ 20.0f,26.0f,0.0f };
};
