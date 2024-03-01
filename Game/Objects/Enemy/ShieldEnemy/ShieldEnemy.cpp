#include "ShieldEnemy.h"

void ShieldEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::CYAN);
}

void ShieldEnemy::Update()
{

}

void ShieldEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation.x += MoveVec.y;
}
