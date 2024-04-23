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
	sprite.isActive = true;
	sprite.name = "HPMainSprite";
	//sprite.commonColor = new LWP::Utility::Color("");
	sprite.isActive = true;
}

bool EXLife::Execution()
{
	ImGui::Begin("EXLife");
	ImGui::Text("%d",EXLife_);
	ImGui::Text("%f",time_);
	if (ImGui::Button("MuinusHp"))EXLife_--;
	ImGui::End();
	if (EXLife_ == 1) {
		sprite.transform.scale = { 1.05f,1.05f };
		sprite.transform.translation = { 20.0f,20.0f,0.0f };
		//lwp::Vector2 size = subSprite.texture.t->GetTextureSize();
		sprite.transform.translation.x = 20.0f + 1 * (1.05f - 1.0f) * 0.5f;
		//player_->GetPlayerParameter()->Hp.hp_
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
}
