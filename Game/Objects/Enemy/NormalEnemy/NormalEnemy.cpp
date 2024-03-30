#include "NormalEnemy.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP::Object::Collider;
void NormalEnemy::Init()
{
	models_.emplace_back();
	models_[0].LoadFile("cube/cube.obj");
	models_[0].commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);
	models_[0].name = "NormalEnemy!!";

	isActive_ = true;

	attackWaitTime_ = kAttackWaitTime;

}

void NormalEnemy::Update()
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

void NormalEnemy::SetPosition(lwp::Vector3 pos)
{
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void NormalEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[0].transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[0].transform.translation += MoveVec * kMove * LWP::Info::GetDeltaTime();
}

void NormalEnemy::Attack()
{
	if (attackWaitTime_ <= 0) {
		attackWork.flag = true;

		Rot = models_[0].transform.rotation;
		attackWork.targetpoint = Rot;
		// 回転を足す
		attackWork.targetpoint.x += -0.5f;
		EndRot = attackWork.targetpoint;
		attackWaitTime_ = kAttackWaitTime;
		Aim();
	}
}

void NormalEnemy::AttackAnimation()
{
	lwp::Vector3 point = { 0.0f,0.0f,0.0f };
	if (attackWork.flag) {
		if (attackWork.t < 1.0f) {
			attackWork.t += attackWork.speed;
			point = lwp::Vector3::Lerp(Rot, attackWork.targetpoint, attackWork.t);
			models_[0].transform.rotation = point;
		}
		else if (attackWork.t >= 1.0f) {
			attackWork.flag = false;
			attackWork.t = 0.0f;

			attackStanbyWork.flag = true;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
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
			point = lwp::Vector3::Lerp(EndRot, Rot, attackEndWork.t);
			models_[0].transform.rotation = point;
		}
		else if (attackEndWork.t >= 1.0f) {
			attackEndWork.flag = false;
			attackEndWork.t = 0.0f;
			isAttack = false;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}


}

bool NormalEnemy::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

void NormalEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[0].transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[0].transform.translation.z);
	models_[0].transform.rotation.y = radian;
}