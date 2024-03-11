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

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	
	followCamera_->SetTarget(player_->GetWorldTransform());
	player_->SetCameraPointer(mainCamera);

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->Init();
}

// 更新
void TItleScene::Update()
{

	// プレイヤー
	player_->Update();

	// 追従カメラ
	followCamera_->Update();

	mainCamera->transform = followCamera_->viewProjection_.transform;

	enemyManager_->Update();

	
}