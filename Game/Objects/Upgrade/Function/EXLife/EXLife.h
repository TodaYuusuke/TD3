#pragma once
#include <Adapter.h>
class EXLife
{
public:
	void Init();
	bool Execution();

	void DecreaseHP();

private:
	float time_ = 0;
	uint32_t EXLife_ = 1;
	const float kHealInterval = 60.0f;
};
