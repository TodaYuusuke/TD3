#include "Weapon.h"
void Weapon::Initialize()
{
	// モデル読み込み
	demoModel_ = LWP::Resource::LoadModel("cube/cube.obj");
	//demoModel_->transform.Parent(&world_);
	demoModel_->isActive = true;
	demoModel_->name = "PlayerWeapon";

	demoModel_->transform.translation.z = 1.0f;
	demoModel_->transform.scale = { 0.5f,0.5f,0.8f };

	InitDatas();

	behavior_ = Behavior::Root;
}

void Weapon::Update()
{
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
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
}

void Weapon::InitRoot()
{
	rootData_.translate_.start_ = demoModel_->transform.translation;
	rootData_.rotate_.start_ = demoModel_->transform.rotation;
}

void Weapon::InitSlash()
{
}

void Weapon::InitMoment()
{
	momentData_.scale_.start_ = demoModel_->transform.scale;
	momentData_.rotate_.start_ = demoModel_->transform.rotation;
	momentData_.translate_.start_ = demoModel_->transform.translation;
}

void Weapon::UpdateRoot()
{
	demoModel_->transform.scale = lwp::Vector3::Slerp(rootData_.scale_.start_, rootData_.scale_.end_, *pT_);
	demoModel_->transform.rotation = lwp::Vector3::Slerp(rootData_.rotate_.start_, rootData_.rotate_.end_, *pT_);
	demoModel_->transform.translation = lwp::Vector3::Slerp(rootData_.translate_.start_, rootData_.translate_.end_, *pT_);
}

void Weapon::UpdateSlash()
{
	demoModel_->transform.scale = lwp::Vector3::Slerp(slashData_.scale_.start_, slashData_.scale_.end_, *pT_);
	demoModel_->transform.rotation = lwp::Vector3::Slerp(slashData_.rotate_.start_, slashData_.rotate_.end_, *pT_);
	demoModel_->transform.translation = lwp::Vector3::Slerp(slashData_.translate_.start_, slashData_.translate_.end_, *pT_);
}

void Weapon::UpdateMoment()
{
	demoModel_->transform.scale = lwp::Vector3::Slerp(momentData_.scale_.start_, momentData_.scale_.end_, *pT_);
	demoModel_->transform.rotation = lwp::Vector3::Slerp(momentData_.rotate_.start_, momentData_.rotate_.end_, *pT_);
	demoModel_->transform.translation = lwp::Vector3::Slerp(momentData_.translate_.start_, momentData_.translate_.end_, *pT_);
}

void Weapon::InitDatas()
{
	InitRootData();
	InitSlashData();
	InitMomentData();
}

void Weapon::InitRootData()
{
	rootData_.scale_.start_ = { 0.5f,0.5f,0.8f };
	rootData_.scale_.end_ = { 0.5f,0.5f,0.8f };
	rootData_.rotate_.start_ = { 0.0f,0.0f,0.0f };
	rootData_.rotate_.end_ = { 0.0f,0.0f,0.0f };
	rootData_.translate_.start_ = { 0.5f,0.0f,0.0f };
	rootData_.translate_.end_ = { -0.5f,0.0f,0.0f };
}

void Weapon::InitSlashData()
{
	slashData_.scale_.start_ = { 0.5f,0.5f,0.8f };
	slashData_.scale_.end_ = { 0.5f,0.5f,1.0f };
	slashData_.rotate_.start_ = { 0.0f,0.0f,0.0f };
	slashData_.rotate_.end_ = { 0.0f,0.0f,0.0f };
	slashData_.translate_.start_ = { -1.0f,0.5f,3.0f };
	slashData_.translate_.end_ = { 1.0f,0.5f,1.0f };
}

void Weapon::InitMomentData()
{
	momentData_.scale_.start_ = { 0.5f,0.5f,1.0f };
	momentData_.scale_.end_ = { 0.5f,0.5f,0.8f };
	momentData_.rotate_.start_ = { 0.0f,0.0f,0.0f };
	momentData_.rotate_.end_ = { 0.0f,0.0f,0.0f };
	momentData_.translate_.start_ = { 0.0f,0.0f,0.0f };
	momentData_.translate_.end_ = { -0.5f,0.0f,0.0f };
}
