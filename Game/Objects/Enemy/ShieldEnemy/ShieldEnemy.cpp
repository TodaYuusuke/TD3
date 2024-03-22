#include "ShieldEnemy.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP::Object::Collider;

void ShieldEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::CYAN);
	isActive_ = true;

	attackWaitTime_ = kAttackWaitTime;
}

void ShieldEnemy::Update()
{
	if (CheckAttackRange()) {
		isAttack = true;
	}
	if (isAttack) {
		// 攻撃処理
		Attack();
		// 攻撃アニメーション
		AttackAnimation();
	}
	else {
		Aim();
		Move();
	}

	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}
}

void ShieldEnemy::SetPosition(lwp::Vector3 pos)
{
	models_[0]->transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void ShieldEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[0]->transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[0]->transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTime();
}

void ShieldEnemy::Attack()
{
	if (attackWaitTime_ <= 0) {
		attackWork.flag = true;
			lwp::Vector3 point{ 0.0f,0.0f,-1.0f };
		attackWork.targetpoint = (point * lwp::Matrix4x4::CreateRotateXYZMatrix(models_[0]->transform.rotation))/*ベクトルを反転*/;
		attackWork.targetpoint = attackWork.targetpoint.Normalize();
		attackEndWork.targetpoint = attackWork.targetpoint * -1/*ベクトルを反転*/;
		attackWaitTime_ = kAttackWaitTime;
		Aim();
	}
}

void ShieldEnemy::AttackAnimation()
{
	// 攻撃アニメーション
	if (attackWork.flag) {
		if (attackWork.t < 1.0f) {
			attackWork.t += attackWork.speed;
			models_[0]->transform.translation += attackWork.targetpoint * LWP::Info::GetDeltaTime();
		}
		else if (attackWork.t >= 1.0f) {
			attackWork.flag = false;
			attackWork.t = 0.0f;

			attackStanbyWork.flag = true;
			collider_->mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
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

			models_[0]->transform.translation += attackEndWork.targetpoint * LWP::Info::GetDeltaTime() * 10.0f;
		}
		else if (attackEndWork.t >= 1.0f) {
			attackEndWork.flag = false;
			attackEndWork.t = 0.0f;
			isAttack = false;
			collider_->mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}

}

bool ShieldEnemy::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0]->transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

void ShieldEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[0]->transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[0]->transform.translation.z);
	models_[0]->transform.rotation.y = radian;
}