#include "ArrowEnemy.h"

void ArrowEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	arrowmodels_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);
}

void ArrowEnemy::Update()
{
	if (lwp::Keyboard::GetTrigger(DIK_SPACE)) {
		Arrow* arrow = new Arrow();
 		arrow->Init(arrowmodels_, models_[0]->transform.translation);
		arrows_.push_back(arrow);
	}

	for (Arrow* arrows : arrows_) {
		arrows->Update();
	}
}

void ArrowEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation.x -= MoveVec.y * LWP::Info::GetDeltaTime();
}

void ArrowEnemy::Attack()
{
	
}
