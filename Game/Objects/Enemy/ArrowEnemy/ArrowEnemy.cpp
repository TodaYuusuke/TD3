#include "ArrowEnemy.h"
#include "../../Player/Player.h"
#include <math.h>

using namespace LWP::Object::Collider;

void ArrowEnemy::Init()
{
	// これでできる
	models_.emplace_back(); 
	models_[0].LoadFile("cube/cube.obj");
	models_[0].commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::GREEN);
	models_[0].name = "ArrowEnemy!!";

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
	models_[0].transform.translation = pos + player_->GetWorldTransform()->GetWorldPosition();
}

void ArrowEnemy::CreateCollider()
{
	// 当たり判定を設定
	collider_ = LWP::Object::Collider::AABB();
	// 当たり判定を取る
	collider_.CreateFromPrimitive(&models_[0]);
	// マスク処理
	collider_.mask.SetBelongFrag(MaskLayer::Enemy | MaskLayer::Layer2);
	collider_.mask.SetHitFrag(MaskLayer::Layer3);
	// 今のところは何もしていない
	collider_.SetOnCollisionLambda([this](HitData data) {
		data;
		if (data.state == OnCollisionState::Press && isActive_ &&
			data.hit &&
			(data.hit->mask.GetBelongFrag() & MaskLayer::Layer3))
		{
			isActive_ = false;
			//models_[0].isActive = false;
			//collider_.isActive = false;
			arrows_.remove_if([](Arrow* arrow) {
				arrow->Death();
				delete arrow;
				return true;
				});
		}
		});
}

void ArrowEnemy::Move()
{
	lwp::Vector3 MoveVec = player_->GetWorldTransform()->translation - models_[0].transform.translation;
	MoveVec = MoveVec.Normalize();
	MoveVec.y = 0.0f;
	models_[0].transform.translation += MoveVec * 2.0f * LWP::Info::GetDeltaTime();
}

void ArrowEnemy::Attack()
{
	// 矢の発射
	if (attackWaitTime_ <= 0)
	{
		Aim();
		Arrow* arrow = new Arrow();
		arrow->Init(models_[0].transform);
		arrows_.push_back(arrow);
		attackWaitTime_ = kAttackWaitTime;
		isAttack = false;

	}
}

void ArrowEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(player_->GetWorldTransform()->GetWorldPosition().x - models_[0].transform.translation.x, player_->GetWorldTransform()->GetWorldPosition().z - models_[0].transform.translation.z);
	models_[0].transform.rotation.y = radian;
}

bool ArrowEnemy::CheckAttackRange() {
	// 自機との距離
	float distance = (models_[0].transform.translation - player_->GetWorldTransform()->translation).Length();
	if (distance <= kAttackRange) {
		return true;
	}
	return false;
}
