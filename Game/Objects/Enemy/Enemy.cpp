#include "Enemy.h"

void Enemy::Initialize()
{
	world_.Initialize();
	models_.push_back(LWP::Resource::LoadModel("cube/cube.obj"));

}

void Enemy::Update()
{
	if (lwp::Keyboard::GetPress(DIK_W)) {
		Move({0.0f,0.1f,0.0f});
	}
	if (lwp::Keyboard::GetPress(DIK_S)) {
		Move({0.0f,-0.1f,0.0f});
	}
}

void Enemy::Move(LWP::Math::Vector3 MoveVec)
{
	models_[0]->transform.translation += MoveVec;
}
