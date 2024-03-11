#include "NormalEnemy.h"
#include "../../Player/Player.h"

void NormalEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);

	attackWaitTime_ = kAttackWaitTime;
}

void NormalEnemy::Update()
{
	if (CheckAttackRange()) {
		Attack();
	}
	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
}

void NormalEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation += MoveVec * LWP::Info::GetDeltaTime();
}

void NormalEnemy::Attack()
{
	if (attackWaitTime_ <= 0) {
		attackWork.flag = true;
		PlayerRot = models_[0]->transform.rotation;
		attackWaitTime_ = kAttackWaitTime;
	}
	//if (lwp::Keyboard::GetPress(DIK_SPACE)) {
	//	attackWork.flag = true;
	//	PlayerRot = models_[0]->transform.rotation;
	//}
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

bool NormalEnemy::CheckAttackRange() {
	// Ž©‹@‚Æ‚Ì‹——£
	float distance = (models_[0]->transform.translation - player_->GetWorldPosition()).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

LWP::Math::Vector3 NormalEnemy::GetDirectVel() {
	return (models_[0]->transform.translation - player_->GetWorldPosition()).Normalize();
}