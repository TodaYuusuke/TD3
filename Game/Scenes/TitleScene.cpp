#include "TitleScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void TItleScene::Initialize()
{
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetScene(this);

	// 地面
	Mesh* ground = LWP::Resource::LoadModel("ground/ground.obj");
	ground->transform.translation.y = -0.5f;
	ground->transform.scale = { 10.0f,0.1f, 10.0f };
	ground->name = "Ground";

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetCameraAddress(mainCamera);
	followCamera_->SetTarget(player_->GetWorldTransform());
	followCamera_->SetPlayer(player_.get());

	player_->SetCameraPointer(followCamera_.get());

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->Init();
}

// 更新
void TItleScene::Update()
{
	// スローを確認
	if (isJustSlash_)
	{
		time_ += lwp::GetDeltaTime();
		if (cTIMESLOW_ <= time_)
		{
			isJustSlash_ = false;
			player_->EndJust();
		}
	}

	// プレイヤー
	player_->Update();

	// 追従カメラ
	followCamera_->Update();

	enemyManager_->Update();
}

void TItleScene::StartJustSlash()
{
	isJustSlash_ = true;
	time_ = 0.0f;
	LWP::Info::SetDeltaTimeMultiply(0.1f);
	followCamera_->StartSlash();
}
