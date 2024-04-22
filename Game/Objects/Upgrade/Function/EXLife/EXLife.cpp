#include "EXLife.h"

void EXLife::Init()
{
	time_ = 0;
	EXLife_ = 1;
}

bool EXLife::Execution()
{
	if (EXLife_ == 0) {
		time_ += 1.0f / 60.0f;
	}
	if (time_ >= 60.0f) {
		EXLife_ += 1;
	}
	return true;
}

void EXLife::DecreaseHP()
{
	EXLife_ -= 1;
}
