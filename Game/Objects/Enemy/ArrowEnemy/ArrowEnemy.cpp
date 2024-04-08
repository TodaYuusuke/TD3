#include "ArrowEnemy.h"
#include "../../Player/Player.h"
#include <math.h>

using namespace LWP::Object::Collider;

void ArrowEnemy::Init()
{
	// これでできる
	models_.emplace_back();
	models_[0].LoadFile("ArrowEnemy/ArrowEnemy.obj");
	models_[0].name = "ArrowEnemy!!";

	// 最初から描画
	isActive_ = true;
	// 当たり判定は Initialize() 関数で呼び出しているので
	// 条件を変えたいなら CreateCollider() 関数をオーバーライドする
	attackWaitTime_ = kAttackWaitTime;

	// HP を設定
	hp_ = 30;
}

void ArrowEnemy::Update()
{
	// 矢の更新処理
	arrows_.remove_if([](Arrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		arrow->Update();
		return false;
		});

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
		// 矢の発射
		if (attackWaitTime_ > 60)
		{
			Aim();
		}
		else if (attackWaitTime_ <= 0)
		{
			Attack();
		}
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

void ArrowEnemy::SetPosition(lwp::Vector3 pos)
{
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void ArrowEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[0].transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[0].transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTimeF();
}

void ArrowEnemy::Attack()
{
	Arrow* arrow = new Arrow();
	arrow->Init(models_[0].transform);
	arrows_.push_back(arrow);
	attackWaitTime_ = kAttackWaitTime;
	isAttack = false;
}

void ArrowEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[0].transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[0].transform.translation.z);

	models_[0].transform.rotation.y = LerpShortAngle(models_[0].transform.rotation.y, radian, 0.5f);
}

bool ArrowEnemy::CheckAttackRange()
{
	// 自機との距離
	float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange)
	{
		return true;
	}
	return false;
}

float ArrowEnemy::LerpShortAngle(float a, float b, float t)
{
	// 角度差分を求める
	float diff = b - a;

	float pi = 3.14f;

	diff = std::fmod(diff, 2 * pi);
	if (diff < 2 * -pi)
	{
		diff += 2 * pi;
	}
	else if (diff >= 2 * pi)
	{
		diff -= 2 * pi;
	}

	diff = std::fmod(diff, 2 * pi);
	if (diff < -pi)
	{
		diff += 2 * pi;
	}
	else if (diff >= pi)
	{
		diff -= 2 * pi;
	}

	return a + diff * t;
}
