#include "EXLife.h"
#include "Game/Objects/Player/Player.h"
void EXLife::Init(Player* Player)
{
	time_ = 0;
	EXLife_ = 1;
	player_ = Player;
	// スプライト
	sprite.texture = LWP::Resource::LoadTexture("white.png");
	sprite.anchorPoint = { 0.0f,0.0f };
	sprite.isUI = true;
	sprite.isActive = false;
	sprite.name = "HPMainSprite";
}

bool EXLife::Execution()
{
	if (EXLife_ == 1) {
		sprite.transform.translation.x = (256.0f * 0.5f) * player_->GetPlayerParameter()->Hp.hp_;
		sprite.transform.translation.y = basePosition.y;
		sprite.transform.translation.y = basePosition.y;
		sprite.isActive = true;
		ImGui::Begin("EXLife On");
		ImGui::End();
		return true;
	}

	if (EXLife_ == 0) {
		time_ += 1.0f / 60.0f;
	}
	if (time_ >= kHealInterval) {
		EXLife_ += 1;
		sprite.isActive = true;
		time_ = 0;
		return true;
	}
	return false;

}

void EXLife::DecreaseHP()
{
	EXLife_ -= 1;
	sprite.isActive = false;
}
