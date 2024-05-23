#include "EXLife.h"
#include "Game/Objects/Player/Player.h"
void EXLife::Init(Player* Player)
{
	time_ = 0;
	EXLife_ = 1;
	player_ = Player;
	// スプライト
	sprite.texture = LWP::Resource::LoadTexture("EXLife.png");
	sprite.anchorPoint = { 0.0f,0.0f };
	sprite.isUI = true;
	sprite.isActive = false;
	sprite.name = "HPMainSprite";
	sprite.transform.scale = { 0.5f,0.5f };
}

bool EXLife::Update()
{
	sprite.transform.translation.x = (256.0f * 0.5f) * player_->GetPlayerParameter()->Hp.hp_;
	sprite.transform.translation.y = basePosition.y;
	sprite.transform.translation.y = basePosition.y;
	if (EXLife_ == 1)
	{
		return sprite.isActive = true;
	}

	if (EXLife_ == 0)
	{
		time_ += 1.0f / 60.0f;
		if (time_ >= kHealInterval)
		{

			EXLife_ += 1;
			time_ = 0;
			return sprite.isActive = true;
		}
	}
	return false;

}

void EXLife::DecreaseHP()
{
	EXLife_ -= 1;
	sprite.isActive = false;
}
