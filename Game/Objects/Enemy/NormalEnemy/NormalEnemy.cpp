#include "NormalEnemy.h"

using namespace LWP::Object::Collider;
void NormalEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);
	
	isActive_ = true;

	// 当たり判定を設定
	collider_ = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	// 当たり判定を取る
	collider_->CreateFromPrimitive(models_[0]);
	// 今のところは何もしていない
	collider_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		if (data.state == OnCollisionState::Trigger && isActive_)
		{
			isActive_ = false;
			models_[0]->isActive = false;
			collider_->isActive = false;
		}
		});
}

void NormalEnemy::Update()
{
	Attack();
}

void NormalEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation += MoveVec * LWP::Info::GetDeltaTime();
}

void NormalEnemy::Attack()
{
	if (lwp::Keyboard::GetPress(DIK_SPACE)) {
		attackWork.flag = true;
		PlayerRot = models_[0]->transform.rotation;
	}
	lwp::Vector3 point = { 0.0f,0.0f,0.0f };
	if (attackWork.flag) {
		if (attackWork.t < 1.0f) {
			attackWork.t += attackWork.speed;
			point = lwp::Vector3::Lerp(PlayerRot,attackWork.targetpoint,attackWork.t);
			models_[0]->transform.rotation = point;
		}
		else if (attackWork.t >= 1.0f) {
			attackWork.flag = false;
			attackWork.t = 0.0f;

			attackStanbyWork.flag = true;
		}
	}
	if (attackStanbyWork.flag) {
		attackStanbyWork.t += attackStanbyWork.speed;
		if (attackStanbyWork.t >= 1.0f) {
			attackStanbyWork.flag = false;
			attackStanbyWork.t = 0.0f;

			attackEndWork.flag = true;
		}
	}
	if (attackEndWork.flag) {
		if (attackEndWork.t < 1.0f) {
			attackEndWork.t += attackEndWork.speed;
			point = lwp::Vector3::Lerp(PlayerRot, attackEndWork.targetpoint, attackEndWork.t);
			models_[0]->transform.rotation = point;
		}
		else if (attackEndWork.t >= 1.0f) {
			attackEndWork.flag = false;
			attackEndWork.t = 0.0f;

			attackEndWork.flag = false;
		}
	}
}
