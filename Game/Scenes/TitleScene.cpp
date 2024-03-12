#include "TitleScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void TItleScene::Initialize()
{
	uvTexture = LWP::Resource::LoadTextureLongPath("resources/system/texture/uvChecker.png");
	// 球
	//sphere = LWP::Primitive::CreateInstance<Sphere>();
	//sphere->Radius(0.3f);
	//sphere->material.shininess = 40.0f;
	//sphere->texture = uvTexture;

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	mainCamera->transform.Parent(&followCamera_->camera_->transform);

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	
	followCamera_->SetTarget(player_->GetWorldTransform());
	player_->SetCameraPointer(followCamera_->camera_);

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->Init();

	// 地面
	Mesh* ground = LWP::Resource::LoadModel("ground/ground.obj");
	ground->transform.translation.y = -0.5f;
	ground->transform.scale = { 10.0f,0.1f, 10.0f };
	//ground->material.enableLighting = true;
	ground->name = "ground";
}

// 更新
void TItleScene::Update()
{

	// プレイヤー
	player_->Update();

	// 追従カメラ
	followCamera_->Update();

	//mainCamera->transform = followCamera_->camera_->transform;

	enemyManager_->Update();

	
}