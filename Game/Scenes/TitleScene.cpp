#include "TitleScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void TItleScene::Initialize()
{
	// 球
	sphere = LWP::Primitive::CreateInstance<Sphere>();
	sphere->Radius(0.3f);
	sphere->material.shininess = 40.0f;
	sphere->texture = uvTexture;

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&sphere->transform);

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init();
}

// 更新
void TItleScene::Update()
{
	// 追従カメラ
	followCamera_->Update();
	mainCamera->transform = followCamera_->viewProjection_.transform;

	enemyManager_->Update();

	
}