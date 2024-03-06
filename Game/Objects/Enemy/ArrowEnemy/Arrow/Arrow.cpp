#include "Arrow.h"

void Arrow::Init(std::vector<LWP::Primitive::IPrimitive*> models,lwp::WorldTransform transform)
{
	models_ = models;
	models_[0]->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::WHITE);
	models_[0]->transform.translation = transform.translation;
	models_[0]->transform.rotation = transform.rotation;
	models_[0]->transform.scale = { 0.5f,0.5f,1.0f };
	lwp::Vector3 point = { 0.0f,0.0f,1.0f };
	attackWork.targetpoint = point * lwp::Matrix4x4::CreateRotateXYZMatrix(transform.rotation);
	attackWork.targetpoint = attackWork.targetpoint.Normalize();
}

void Arrow::Update()
{
	Attack();
}

void Arrow::Attack()
{

	models_[0]->transform.translation += attackWork.targetpoint * LWP::Info::GetDeltaTime();
	attackWork.flag = true;
}

