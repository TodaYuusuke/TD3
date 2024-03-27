#include "DashBoss.h"
#include "Game/Objects/Player/Player.h"

void DashBoss::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::GREEN);
	models_[0]->transform.scale = { 1,2,1 };

	isActive_ = true;
}

void DashBoss::Update()
{
	// Ž©‹@‚ð‘_‚¤
	Aim();
	// ˆÚ“®ˆ—
	Move();

	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
}

void DashBoss::SetPosition(lwp::Vector3 pos)
{
	models_[0]->transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void DashBoss::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[0]->transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[0]->transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTime();
}

void DashBoss::Attack()
{

}

bool DashBoss::CheckAttackRange() {
	// Ž©‹@‚Æ‚Ì‹——£
	float distance = (models_[0]->transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

LWP::Math::Vector3 DashBoss::GetDirectVel() {
	return (models_[0]->transform.translation - player_->GetWorldTransform()->translation).Normalize();
}

void DashBoss::Aim()
{
	// ‘_‚¤‘ÎÛ‚Ég‘Ì‚ðŒü‚¯‚é
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[0]->transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[0]->transform.translation.z);
	models_[0]->transform.rotation.y = radian;
}