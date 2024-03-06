#include "ArrowEnemy.h"
#include <math.h>

void ArrowEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);
}

void ArrowEnemy::Update()
{
#ifdef _DEBUG
	// –î‚Ì”­Ë
	if (lwp::Keyboard::GetTrigger(DIK_SPACE)) {
		Arrow* arrow = new Arrow();
		arrow->Init(models_[0]->transform);
		arrows_.emplace_back(arrow);
	}
	// ‘Ì‚ÌŒü‚«‚ğ•Ï‚¦‚é
	if (lwp::Keyboard::GetPress(DIK_A)) {
		models_[0]->transform.rotation.y += 0.01f;
	}
	if (lwp::Keyboard::GetPress(DIK_D)) {
		models_[0]->transform.rotation.y -= 0.01f;
	}
#endif // _DEBUG
	// ‘_‚¤
	Aim();

	// –î‚ÌXVˆ—
	for (std::unique_ptr<Arrow>& arrows : arrows_) {
		arrows->Update();
	}

	arrows_.remove_if([](std::unique_ptr<Arrow>& arrow) {
		if (!arrow->GetIsAlive()) {
			arrow.reset();
			return true;
		}
		return false;
		});
}

void ArrowEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation.x -= MoveVec.y * LWP::Info::GetDeltaTime();
}

void ArrowEnemy::Attack()
{

}

void ArrowEnemy::Aim()
{
	// ‘_‚¤‘ÎÛ‚Ég‘Ì‚ğŒü‚¯‚é
	float radian = atan2(target_->x - models_[0]->transform.translation.x, target_->y - models_[0]->transform.translation.y);
	models_[0]->transform.rotation.y = radian;
}
