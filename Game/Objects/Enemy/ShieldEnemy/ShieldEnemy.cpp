#include "ShieldEnemy.h"

void ShieldEnemy::Init()
{
	models_.push_back(LWP::Common::CreateInstance<LWP::Primitive::Cube>());
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::CYAN);
	// 当たり判定を設定
	lwp::Collider::AABB* aabb = LWP::Common::CreateInstance<lwp::Collider::AABB>();
	// 当たり判定を取る
	aabb->CreateFromPrimitive(models_[0]);
	// 今のところは何もしていない
	aabb->SetOnCollisionLambda([](lwp::Collider::ICollider* self, lwp::Collider::ICollider* hit, lwp::Collider::OnCollisionState state) {
		self;
		hit;
		state;
		});
}

void ShieldEnemy::Update()
{
	Attack();
}

void ShieldEnemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation.x += MoveVec.y * LWP::Info::GetDeltaTime();
}

void ShieldEnemy::Attack()
{
	if (lwp::Keyboard::GetPress(DIK_SPACE)) {
		attackWork.flag = true;
		lwp::Vector3 point{ 0.0f,0.0f,-1.0f };
		attackWork.targetpoint = (point * lwp::Matrix4x4::CreateRotateXYZMatrix(models_[0]->transform.rotation)) * -1/*ベクトルを反転*/;
		attackWork.targetpoint = attackWork.targetpoint.Normalize();
		attackEndWork.targetpoint = attackWork.targetpoint * -1/*ベクトルを反転*/;
	}
	if (attackWork.flag) {
		if (attackWork.t < 1.0f) {
			attackWork.t += attackWork.speed;
			models_[0]->transform.translation += attackWork.targetpoint * LWP::Info::GetDeltaTime();
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
			
			models_[0]->transform.translation += attackEndWork.targetpoint * LWP::Info::GetDeltaTime() * 10.0f;
		}
		else if (attackEndWork.t >= 1.0f) {
			attackEndWork.flag = false;
			attackEndWork.t = 0.0f;

			attackEndWork.flag = false;
		}
	}
}
