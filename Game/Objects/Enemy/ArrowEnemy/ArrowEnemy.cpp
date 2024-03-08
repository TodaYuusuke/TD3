#include "ArrowEnemy.h"
#include <math.h>

using namespace LWP::Object::Collider;

void ArrowEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLACK);

	// 最初から描画
	isActive_ = true;
	// 当たり判定は Initialize() 関数で呼び出しているので
	// 条件を変えたいなら CreateCollider() 関数をオーバーライドする
}

void ArrowEnemy::Update()
{
#ifdef _DEBUG
	// 矢の発射
	if (lwp::Keyboard::GetTrigger(DIK_V)) {
		Arrow* arrow = new Arrow();
		arrow->Init(models_[0]->transform);
		arrows_.push_back(arrow);
	}
	// 体の向きを変える
	if (lwp::Keyboard::GetPress(DIK_A)) {
		models_[0]->transform.rotation.y += 0.01f;
	}
	if (lwp::Keyboard::GetPress(DIK_D)) {
		models_[0]->transform.rotation.y -= 0.01f;
	}
#endif // _DEBUG
	// 狙う
	//Aim();

	// 矢の更新処理
	for (Arrow* arrow : arrows_) {
		arrow->Update();
	}

	arrows_.remove_if([](Arrow* arrow) {
		if (!arrow->GetIsAlive()) {
			delete arrow;
			return true;
		}
		return false;
		});
}

void ArrowEnemy::CreateCollider()
{
	// 当たり判定を設定
	collider_ = LWP::Common::CreateInstance<AABB>();
	// 当たり判定を取る
	collider_->CreateFromPrimitive(models_[0]);
	// 今のところは何もしていない
	collider_->SetOnCollisionLambda([this](HitData data) {
		data;
		if (data.state == OnCollisionState::Trigger && isActive_)
		{
			isActive_ = false;
			models_[0]->isActive = false;
			collider_->isActive = false;
		}
		});
}

void ArrowEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation.x -= MoveVec.y * LWP::Info::GetDeltaTime();
}

void ArrowEnemy::Attack()
{

}

void ArrowEnemy::Aim()
{
	// 狙う対象に身体を向ける
	float radian = atan2(target_->x - models_[0]->transform.translation.x, target_->y - models_[0]->transform.translation.y);
	models_[0]->transform.rotation.y = radian;
}
