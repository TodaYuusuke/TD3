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
	player_->destinate_ = direct.Normalize() * 0.75f;

	// コントローラーの入力を合わせる
	float x = LWP::Input::Controller::GetLStick().x;
	float y = LWP::Input::Controller::GetLStick().y;
	if ((player_->destinate_.x < 0 ? -player_->destinate_.x : player_->destinate_.x)
		< (x < 0 ? -x : x))
	{
		player_->destinate_.x = x;
	}
	if ((player_->destinate_.z < 0 ? -player_->destinate_.z : player_->destinate_.z)
		< (y < 0 ? -y : y))
	{
		player_->destinate_.z = y;
	}
	player_->destinate_ = player_->destinate_.Normalize();

	// 方向がゼロだった場合は元に戻す
	if (player_->destinate_.x == 0 && player_->destinate_.z == 0)
	{
		player_->destinate_ = direct.Normalize();
	}

	// そもそも移動入力が無かったらフラグを立てない
	player_->isInputMove_ = !(player_->destinate_.x == 0 && player_->destinate_.z == 0);
}

void IStatus::CheckInputSlash()
{
	if (Keyboard::GetTrigger(DIK_SPACE) ||
		Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		player_->isInputSlash_ = true;
	}
	else
		player_->isInputSlash_ = false;
}
