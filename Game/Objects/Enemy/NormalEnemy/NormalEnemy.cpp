#include "NormalEnemy.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP::Object::Collider;
void NormalEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[Model::Body].LoadFile("NormalEnemy/Body/Body.obj");
	models_[Model::Body].name = "Normal";
	//models_.emplace_back();
	//models_[Model::L_Arm].LoadFile("NormalEnemy/L_Arm/L_Arm.obj");
	//models_[Model::L_Arm].name = "LArm";
	//models_.emplace_back();
	//models_[Model::R_Arm].LoadFile("NormalEnemy/R_Arm/R_Arm.obj");
	//models_[Model::R_Arm].name = "RArm";

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
	models_[Model::Body].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
	//models_[Model::L_Arm].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
	//models_[Model::R_Arm].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void NormalEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[Model::Body].transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[Model::Body].transform.translation += MoveVec * kMove * LWP::Info::GetDeltaTime();
	//models_[Model::L_Arm].transform.translation += MoveVec * kMove * LWP::Info::GetDeltaTime();
	//models_[Model::R_Arm].transform.translation += MoveVec * kMove * LWP::Info::GetDeltaTime();
}

void NormalEnemy::Attack()
{
	if (attackWaitTime_ <= 0) {
		
	//#pragma region
	//	attackWork[Model::L_Arm].flag = true;
	//	Rot[Model::L_Arm] = models_[Model::L_Arm].transform.rotation;
	//	attackWork[Model::L_Arm].targetpoint = Rot[Model::L_Arm];
	//	// 回転を足す
	//	attackWork[Model::L_Arm].targetpoint.x += -3.14f;
	//	EndRot[Model::L_Arm] = attackWork[Model::L_Arm].targetpoint;
	//#pragma endregion L_Arm
	//#pragma region
	//	attackWork[Model::R_Arm].flag = true;
	//	Rot[Model::R_Arm] = models_[Model::R_Arm].transform.rotation;
	//	attackWork[Model::R_Arm].targetpoint = Rot[Model::R_Arm];
	//	// 回転を足す
	//	attackWork[Model::R_Arm].targetpoint.x += -3.14f;
	//	EndRot[Model::R_Arm] = attackWork[Model::R_Arm].targetpoint;
	//#pragma endregion R_Arm
	#pragma region
		attackWork[Model::Body].flag = true;
		Rot[Model::Body] = models_[Model::Body].transform.rotation;
		attackWork[Model::Body].targetpoint = Rot[Model::Body];
		// 回転を足す
		attackWork[Model::Body].targetpoint.x += -3.14f;
		EndRot[Model::Body] = attackWork[Model::Body].targetpoint;
	#pragma endregion Body

		attackWaitTime_ = kAttackWaitTime;
		Aim();
	}
}

void NormalEnemy::AttackAnimation()
{
	AnimeBody();
	//AnimeL_Arm();
	//AnimeR_Arm();

}

void NormalEnemy::AnimeBody()
{
	lwp::Vector3 point = { 0.0f,0.0f,0.0f };
	if (attackWork[Model::L_Arm].flag) {
		if (attackWork[Model::L_Arm].t < 1.0f) {
			attackWork[Model::L_Arm].t += attackWork[Model::L_Arm].speed;
			point = lwp::Vector3::Lerp(Rot[Model::L_Arm], attackWork[Model::L_Arm].targetpoint, attackWork[Model::L_Arm].t);
			models_[Model::L_Arm].transform.rotation = point;
		}
		else if (attackWork[Model::L_Arm].t >= 1.0f) {
			attackWork[Model::L_Arm].flag = false;
			attackWork[Model::L_Arm].t = 0.0f;

			attackStanbyWork[Model::L_Arm].flag = true;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
		}
	}
	if (attackStanbyWork[Model::L_Arm].flag) {
		attackStanbyWork[Model::L_Arm].t += attackStanbyWork[Model::L_Arm].speed;
		if (attackStanbyWork[Model::L_Arm].t >= 1.0f) {
			attackStanbyWork[Model::L_Arm].flag = false;
			attackStanbyWork[Model::L_Arm].t = 0.0f;

			attackEndWork[Model::L_Arm].flag = true;
		}
	}
	if (attackEndWork[Model::L_Arm].flag) {
		if (attackEndWork[Model::L_Arm].t < 1.0f) {
			attackEndWork[Model::L_Arm].t += attackEndWork[Model::L_Arm].speed;
			point = lwp::Vector3::Lerp(EndRot[Model::L_Arm], Rot[Model::L_Arm], attackEndWork[Model::L_Arm].t);
			models_[Model::L_Arm].transform.rotation = point;
		}
		else if (attackEndWork[Model::L_Arm].t >= 1.0f) {
			attackEndWork[Model::L_Arm].flag = false;
			attackEndWork[Model::L_Arm].t = 0.0f;
			isAttack = false;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}
}

void NormalEnemy::AnimeL_Arm()
{
	/*lwp::Vector3 point = { 0.0f,0.0f,0.0f };
	if (attackWork[Model::L_Arm].flag) {
		if (attackWork[Model::L_Arm].t < 1.0f) {
			attackWork[Model::L_Arm].t += attackWork[Model::L_Arm].speed;
			point = lwp::Vector3::Lerp(Rot[Model::L_Arm], attackWork[Model::L_Arm].targetpoint, attackWork[Model::L_Arm].t);
			models_[Model::L_Arm].transform.rotation = point;
		}
		else if (attackWork[Model::L_Arm].t >= 1.0f) {
			attackWork[Model::L_Arm].flag = false;
			attackWork[Model::L_Arm].t = 0.0f;

			attackStanbyWork[Model::L_Arm].flag = true;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
		}
	}
	if (attackStanbyWork[Model::L_Arm].flag) {
		attackStanbyWork[Model::L_Arm].t += attackStanbyWork[Model::L_Arm].speed;
		if (attackStanbyWork[Model::L_Arm].t >= 1.0f) {
			attackStanbyWork[Model::L_Arm].flag = false;
			attackStanbyWork[Model::L_Arm].t = 0.0f;

			attackEndWork[Model::L_Arm].flag = true;
		}
	}
	if (attackEndWork[Model::L_Arm].flag) {
		if (attackEndWork[Model::L_Arm].t < 1.0f) {
			attackEndWork[Model::L_Arm].t += attackEndWork[Model::L_Arm].speed;
			point = lwp::Vector3::Lerp(EndRot[Model::L_Arm], Rot[Model::L_Arm], attackEndWork[Model::L_Arm].t);
			models_[Model::L_Arm].transform.rotation = point;
		}
		else if (attackEndWork[Model::L_Arm].t >= 1.0f) {
			attackEndWork[Model::L_Arm].flag = false;
			attackEndWork[Model::L_Arm].t = 0.0f;
			isAttack = false;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}*/
}

void NormalEnemy::AnimeR_Arm()
{
	/*lwp::Vector3 point = { 0.0f,0.0f,0.0f };
	if (attackWork[Model::R_Arm].flag) {
		if (attackWork[Model::R_Arm].t < 1.0f) {
			attackWork[Model::R_Arm].t += attackWork[Model::R_Arm].speed;
			point = lwp::Vector3::Lerp(Rot[Model::R_Arm], attackWork[Model::R_Arm].targetpoint, attackWork[Model::R_Arm].t);
			models_[Model::R_Arm].transform.rotation = point;
		}
		else if (attackWork[Model::R_Arm].t >= 1.0f) {
			attackWork[Model::R_Arm].flag = false;
			attackWork[Model::R_Arm].t = 0.0f;

			attackStanbyWork[Model::R_Arm].flag = true;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
		}
	}
	if (attackStanbyWork[Model::R_Arm].flag) {
		attackStanbyWork[Model::R_Arm].t += attackStanbyWork[Model::R_Arm].speed;
		if (attackStanbyWork[Model::R_Arm].t >= 1.0f) {
			attackStanbyWork[Model::R_Arm].flag = false;
			attackStanbyWork[Model::R_Arm].t = 0.0f;

			attackEndWork[Model::R_Arm].flag = true;
		}
	}
	if (attackEndWork[Model::R_Arm].flag) {
		if (attackEndWork[Model::R_Arm].t < 1.0f) {
			attackEndWork[Model::R_Arm].t += attackEndWork[Model::R_Arm].speed;
			point = lwp::Vector3::Lerp(EndRot[Model::R_Arm], Rot[Model::R_Arm], attackEndWork[Model::R_Arm].t);
			models_[Model::R_Arm].transform.rotation = point;
		}
		else if (attackEndWork[Model::R_Arm].t >= 1.0f) {
			attackEndWork[Model::R_Arm].flag = false;
			attackEndWork[Model::R_Arm].t = 0.0f;
			isAttack = false;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}*/
}

bool NormalEnemy::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[Model::Body].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}

void NormalEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[Model::Body].transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[Model::Body].transform.translation.z);
	models_[Model::Body].transform.rotation.y = radian;
	//models_[Model::L_Arm].transform.rotation.y = radian;
	//models_[Model::R_Arm].transform.rotation.y = radian;
}