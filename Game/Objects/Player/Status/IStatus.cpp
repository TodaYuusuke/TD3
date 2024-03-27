#include "IStatus.h"

#include "../Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Math;

void IStatus::Init(Player* p)
{
	assert(p);
	player_ = p;

	motions_.clear();
	// アニメーションの作成
	CreateMotions();
}

void IStatus::Reset()
{
	for (size_t i = 0; i < motions_.size(); i++)
	{
		motions_[i].Start();
	}
}

void IStatus::Update()
{

}

void IStatus::CheckInputMove()
{
	Vector3 direct{ 0.0f,0.0f,0.0f };
	if (Keyboard::GetPress(DIK_W))
	{
		direct.z += 1.0f;
	}
	else if (Keyboard::GetPress(DIK_S))
	{
		direct.z -= 1.0f;
	}
	if (Keyboard::GetPress(DIK_A))
	{
		direct.x -= 1.0f;
	}
	else if (Keyboard::GetPress(DIK_D))
	{
		direct.x += 1.0f;
	}

	// キーボード入力として区別させる
	destinate_ = direct.Normalize() * 0.75f;

	// コントローラーの入力を合わせる
	float x = LWP::Input::Controller::GetLStick().x;
	float y = LWP::Input::Controller::GetLStick().y;
	if ((destinate_.x < 0 ? -destinate_.x : destinate_.x)
		< (x < 0 ? -x : x))
	{
		destinate_.x = x;
	}
	if ((destinate_.z < 0 ? -destinate_.z : destinate_.z)
		< (y < 0 ? -y : y))
	{
		destinate_.z = y;
	}
	destinate_ = destinate_.Normalize();

	// 方向がゼロだった場合は元に戻す
	if (destinate_.x == 0 && destinate_.z == 0)
	{
		destinate_ = direct.Normalize();
	}

	// そもそも移動入力が無かったらフラグを立てない
	isInputMove_ = !(destinate_.x == 0 && destinate_.z == 0);
}