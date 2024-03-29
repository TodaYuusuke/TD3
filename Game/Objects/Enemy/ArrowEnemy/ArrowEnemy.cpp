#include "ArrowEnemy.h"
#include "../../Player/Player.h"
#include <math.h>

using namespace LWP::Object::Collider;

void ArrowEnemy::Init()
{
	models_.push_back(new LWP::Primitive::Cube);
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::GREEN);

	// 最初から描画
	isActive_ = true;
	// 当たり判定は Initialize() 関数で呼び出しているので
	// 条件を変えたいなら CreateCollider() 関数をオーバーライドする
	attackWaitTime_ = kAttackWaitTime;
}

void ArrowEnemy::Update()
{
	if (CheckAttackRange()) {
		isAttack = true;
	}
	if (isAttack) {
		// 攻撃処理
		Attack();
	}
	else {
		Aim();
		Move();
	}
	if (attackWaitTime_ >= 0) {
		attackWaitTime_--;
	}

	// 矢の更新処理
	for (Arrow* arrow : arrows_)
	{
		arrow->Update();
	}

	arrows_.remove_if([](Arrow* arrow) {
		if (!arrow->GetIsAlive())
		{
			arrow->Death();
			delete arrow;
			return true;
		}
		return false;
		});


}

void ArrowEnemy::SetPosition(lwp::Vector3 pos)
{
	models_[0]->transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void ArrowEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[0]->transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[0]->transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTime();
}

void ArrowEnemy::Attack()
{
	// 矢の発射
	if (attackWaitTime_ <= 0)
	{
		Aim();
		Arrow* arrow = new Arrow();
		arrow->Init(models_[0]->transform);
		arrows_.push_back(arrow);
		attackWaitTime_ = kAttackWaitTime;
		isAttack = false;

	}
}

void ArrowEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[0]->transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[0]->transform.translation.z);
	models_[0]->transform.rotation.y = radian;
}

bool ArrowEnemy::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0]->transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}
