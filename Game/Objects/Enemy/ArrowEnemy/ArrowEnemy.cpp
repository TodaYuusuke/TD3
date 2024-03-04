#include "ArrowEnemy.h"

void ArrowEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);
	models_[1]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::WHITE);
	models_[1]->transform.scale = {0.5f,0.5f,0.5f};
}

void ArrowEnemy::Update()
{

}

void ArrowEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation.x -= MoveVec.y * LWP::Info::GetDeltaTime();
}

void ArrowEnemy::Attack()
{

}
