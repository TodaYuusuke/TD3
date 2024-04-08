#include "NormalEnemy.h"
#include "Game/Objects/Player/Player.h"

using namespace LWP::Object::Collider;
void NormalEnemy::Init()
{
	models_.reserve(1);
	models_.emplace_back();
	models_[Model::Body].LoadFile("NormalEnemy/NormalEnemy.obj");
	models_[Model::Body].name = "Normal";

	attackWaitTime_ = kAttackWaitTime;

	// HP を設定
	hp_ = 30;
}

void NormalEnemy::Update()
{
	// 死亡時アニメーション
	// 死んだかどうかはすぐに判別
	if (IsDead_)
	{
		Dying();
		DyingAnimation();
		return;
	}

	// 無敵とかを調べる
	CheckFlags();
	// ここで無量空処されてる時は処理しない
	// アニメーションとかあるなら処理する
	if (isUtopia_)
	{
		return;
	}

	if (CheckAttackRange())
	{
		isAttack = true;
	}
	if (isAttack)
	{
		// 攻撃処理
		Attack();
		// 攻撃アニメーション
		AttackAnimation();
	}
	else
	{
		Aim();
		Move();
	}

	if (attackWaitTime_ >= 0)
	{
		attackWaitTime_--;
	}
}

void NormalEnemy::SetPosition(lwp::Vector3 pos)
{
	models_[Model::Body].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void NormalEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[Model::Body].transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[Model::Body].transform.translation += MoveVec * kMove * LWP::Info::GetDeltaTimeF();
}

void NormalEnemy::Attack()
{
	if (attackWaitTime_ <= 0)
	{
#pragma region
		attackRotWork.flag = true;
		Rot = models_[Model::Body].transform.rotation;
		attackRotWork.targetpoint = Rot;
		EndRot = attackRotWork.targetpoint;
		// 回転を足す
		attackRotWork.targetpoint.y += -3.14f;

		lwp::Vector3 point{ 0.0f,0.0f,-1.0f };
		attackMoveWork.targetpoint = (point * lwp::Matrix4x4::CreateRotateXYZMatrix(models_[0].transform.rotation))/*ベクトルを反転*/;
		attackMoveWork.targetpoint = attackMoveWork.targetpoint.Normalize();
		attackMoveEndWork.targetpoint = attackMoveWork.targetpoint * -1/*ベクトルを反転*/;
#pragma endregion Body

		attackWaitTime_ = kAttackWaitTime;
		Aim();
	}
}

void NormalEnemy::AttackAnimation()
{
	lwp::Vector3 point = { 0.0f,0.0f,0.0f };
	if (attackRotWork.flag)
	{
		if (attackRotWork.t < 1.0f)
		{
			// 回転
			attackRotWork.t += attackRotWork.speed;
			point = lwp::Vector3::Lerp(Rot, attackRotWork.targetpoint, attackRotWork.t);
			models_[Model::Body].transform.rotation = point;
			// 後ろに移動
			models_[Model::Body].transform.translation += attackMoveWork.targetpoint * LWP::Info::GetDeltaTime() * 10.0f;

		}
		else if (attackRotWork.t >= 1.0f)
		{
			attackRotWork.flag = false;
			attackRotWork.t = 0.0f;

			attackStanbyWork.flag = true;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
		}
	}
	if (attackStanbyWork.flag)
	{
		attackStanbyWork.t += attackStanbyWork.speed;
		if (attackStanbyWork.t >= 1.0f)
		{
			attackStanbyWork.flag = false;
			attackStanbyWork.t = 0.0f;

			attackEndWork.flag = true;
		}
	}
	if (attackEndWork.flag)
	{
		if (attackEndWork.t < 1.0f)
		{
			attackEndWork.t += attackEndWork.speed;
			point = lwp::Vector3::Lerp(EndRot, Rot, attackEndWork.t);
			models_[Model::Body].transform.rotation = point;
			// 突進
			models_[Model::Body].transform.translation += attackMoveEndWork.targetpoint * LWP::Info::GetDeltaTime() * 30.0f;


		}
		else if (attackEndWork.t >= 1.0f)
		{
			attackEndWork.flag = false;
			attackEndWork.t = 0.0f;
			isAttack = false;
			collider_.mask.SetBelongFrag(MaskLayer::Enemy);
		}
	}

}

bool NormalEnemy::CheckAttackRange()
{
	// 自機との距離
	float distance = (models_[Model::Body].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange)
	{
		return true;
	}
	return false;
}

void NormalEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[Model::Body].transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[Model::Body].transform.translation.z);
	models_[Model::Body].transform.rotation.y = radian;
}