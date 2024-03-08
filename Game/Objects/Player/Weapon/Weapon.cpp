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
			rootData_->translate_.start_ = demoModel_->transform.translation;
			rootData_->rotate_.start_ = demoModel_->transform.rotation;
			break;
		case Weapon::Behavior::Slash:
			break;
		case Weapon::Behavior::Moment:
			momentData_->scale_.start_ = demoModel_->transform.scale;
			momentData_->rotate_.start_ = demoModel_->transform.rotation;
			momentData_->translate_.start_ = demoModel_->transform.translation;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}
	switch (behavior_)
	{
	case Weapon::Behavior::Root:
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

void Weapon::UpdateRoot()
{
	demoModel_->transform.scale = lwp::Vector3::Slerp(rootData_->scale_.start_, rootData_->scale_.end_, *pT_);
	demoModel_->transform.rotation = lwp::Vector3::Slerp(rootData_->rotate_.start_, rootData_->rotate_.end_, *pT_);
	demoModel_->transform.translation = lwp::Vector3::Slerp(rootData_->translate_.start_, rootData_->translate_.end_, *pT_);
}

void Weapon::UpdateSlash()
{
	demoModel_->transform.scale = lwp::Vector3::Slerp(slashData_->scale_.start_, slashData_->scale_.end_, *pT_);
	demoModel_->transform.rotation = lwp::Vector3::Slerp(slashData_->rotate_.start_, slashData_->rotate_.end_, *pT_);
	demoModel_->transform.translation = lwp::Vector3::Slerp(slashData_->translate_.start_, slashData_->translate_.end_, *pT_);
}

void Weapon::UpdateMoment()
{
	demoModel_->transform.scale = lwp::Vector3::Slerp(momentData_->scale_.start_, momentData_->scale_.end_, *pT_);
	demoModel_->transform.rotation = lwp::Vector3::Slerp(momentData_->rotate_.start_, momentData_->rotate_.end_, *pT_);
	demoModel_->transform.translation = lwp::Vector3::Slerp(momentData_->translate_.start_, momentData_->translate_.end_, *pT_);
}

void Weapon::InitDatas()
{
	rootData_.reset(InitRootData());
	slashData_.reset(InitSlashData());
	momentData_.reset(InitMomentData());
}

Weapon::RootData* Weapon::InitRootData()
{
	RootData* data = new RootData;
	data->scale_.start_ = { 0.5f,0.5f,0.8f };
	data->scale_.end_ = { 0.3f,0.5f,0.8f };
	data->rotate_.start_ = { 0.0f,0.0f,0.0f };
	data->rotate_.end_ = { 0.0f,0.0f,0.0f };
	data->translate_.start_ = { 0.5f,0.0f,0.0f };
	data->translate_.end_ = { -0.5f,0.0f,0.0f };
	return data;
}

Weapon::SlashData* Weapon::InitSlashData()
{
	SlashData* data = new SlashData;
	data->scale_.start_ = { 0.5f,0.5f,0.8f };
	data->scale_.end_ = { 0.5f,0.5f,1.0f };
	data->rotate_.start_ = { 0.0f,0.0f,0.0f };
	data->rotate_.end_ = { 0.0f,0.0f,0.0f };
	data->translate_.start_ = { -2.0f,0.5f,3.0f };
	data->translate_.end_ = { 2.0f,0.0f,1.0f };
	return data;
}

Weapon::MomentData* Weapon::InitMomentData()
{
	MomentData* data = new MomentData;
	data->scale_.start_ = { 0.5f,0.5f,1.0f };
	data->scale_.end_ = { 0.3f,0.5f,0.8f };
	data->rotate_.start_ = { 0.0f,0.0f,0.0f };
	data->rotate_.end_ = { 0.0f,0.0f,0.0f };
	data->translate_.start_ = { 0.0f,0.0f,0.0f };
	data->translate_.end_ = { -0.5f,0.0f,0.0f };
	return data;
}
