#include "EXLife.h"

void EXLife::Init()
{
	time_ = 0;
	EXLife_ = 1;
}

bool EXLife::Execution()
{
	ImGui::Begin("EXLife");
	ImGui::Text("%d",EXLife_);
	ImGui::Text("%f",time_);
	if (ImGui::Button("MuinusHp"))EXLife_--;
	ImGui::End();

	if (EXLife_ == 0) {
		time_ += 1.0f / 60.0f;
	}
	if (time_ >= kHealInterval) {
		EXLife_ += 1;
		time_ = 0;
		return true;
	}
	return true;

}

void EXLife::DecreaseHP()
{
	EXLife_ -= 1;
}
