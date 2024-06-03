#include "Weapon.h"
void Weapon::Initialize()
{
	// モデル読み込み
	demoModel_.LoadFile("Weapon/Weapon.obj");
	demoModel_.isActive = true;
	demoModel_.name = "PlayerWeapon";

	demoModel_.transform.translation.z = 1.0f;
	demoModel_.material.enableLighting = true;

	behavior_ = Behavior::Root;
}

void Weapon::Update()
{
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		t_ = 0.0f;
		prePos_ = demoModel_.transform.translation;
		switch (behavior_)
		{
		case Weapon::Behavior::Root:
			InitRoot();
			break;
		case Weapon::Behavior::Slash:
			InitSlash();
			break;
		case Weapon::Behavior::Moment:
			InitMoment();
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}
	switch (behavior_)
	{
	case Weapon::Behavior::Root:
		// アニメーション来るまで放置
		UpdateRoot();
		break;
	case Weapon::Behavior::Slash:
		UpdateSlash();
		break;
	case Weapon::Behavior::Moment:
		UpdateMoment();
		break;
	default:
		break;
	}
	t_ += lwp::GetDeltaTimeF();
	t_ = std::clamp<float>(t_, 0.0f, 1.0f);
}

void Weapon::InitRoot()
{
	demoModel_.transform.translation = { 0.0f,0.0f,1.0f };
}

void Weapon::InitSlash()
{
}

void Weapon::InitMoment()
{
}

void Weapon::UpdateRoot()
{
}

void Weapon::UpdateSlash()
{
	float easeT = LWP::Utility::Easing::OutExpo(t_);
	demoModel_.transform.translation.z = (10.0f * (easeT)) + (prePos_.z * (1.0f - easeT));
}

void Weapon::UpdateMoment()
{
	float easeT = LWP::Utility::Easing::InOutQuart(t_);
	demoModel_.transform.translation = (lwp::Vector3(0.0f, 0.0f, 1.0f) * (easeT)) + (prePos_ * (1.0f - easeT));
}