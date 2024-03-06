#include "ArrowEnemy.h"

void ArrowEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);
}

void ArrowEnemy::Update()
{
#ifdef _DEBUG
	// 矢の発射
	if (lwp::Keyboard::GetTrigger(DIK_SPACE)) {
		Arrow* arrow = new Arrow();
		arrow->Init(models_[0]->transform);
		arrows_.emplace_back(arrow);
	}
	// 体の向きを変える
	if (lwp::Keyboard::GetPress(DIK_A)) {
		models_[0]->transform.rotation.y += 0.01f;
	}
	if (lwp::Keyboard::GetPress(DIK_D)) {
		models_[0]->transform.rotation.y -= 0.01f;
	}
#endif // _DEBUG

	ImGui::Begin("arrow");
	// 矢の更新処理
	for (std::unique_ptr<Arrow>& arrows : arrows_) {
		arrows->Update();
	}
	ImGui::End();
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
