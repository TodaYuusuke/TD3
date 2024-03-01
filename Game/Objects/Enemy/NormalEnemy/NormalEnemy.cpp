#include "NormalEnemy.h"

void NormalEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::MAGENTA);
}

void NormalEnemy::Update()
{
	
}

void NormalEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation += MoveVec;
}
