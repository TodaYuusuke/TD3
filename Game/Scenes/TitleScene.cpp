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
	ground.LoadFile("ground/ground.obj");
	ground.transform.translation.y = -0.5f;
	ground.transform.scale = { 10.0f,0.1f, 10.0f };
	ground.name = "Ground";

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetCameraAddress(&mainCamera);
	followCamera_->SetTarget(player_->GetWorldTransform());
	followCamera_->SetPlayer(player_.get());
	// カメラを少し上に上げる
	followCamera_->pCamera_->transform.rotation.x = 0.3f;
	player_->SetCameraPointer(followCamera_.get());

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->Init();

	// 経験値タンク
	expManager_ = std::make_unique<ExpManager>();
	expManager_->Initialize();

	// 経験値マネージャーをエネミーマネージャーに設定
	enemyManager_->SetExpManager(expManager_.get());

	// レベル
	level_ = std::make_unique<Level>();
	level_->Initialize(player_->GetWorldTransform()->translation);

	// アップグレード
	//scUpgrade_ = std::make_unique<UpgradeScreen>();
	//scUpgrade_->Initialize();

	//level_->SetUpgradeScreen(scUpgrade_.get());
	upgradeManager_ = std::make_unique<L::UpgradeManager>();
	upgradeManager_->Init();
}

// 更新
void TItleScene::Update()
{
	// スローを確認
	if (player_->GetIsJustSlashing())
	{
		time_ += lwp::GetDeltaTimeF();
		if (cTIMESLOW_ <= time_)
		{
			player_->EndJust();
			EndJustSlash();
		}
	}

	// デバッグ
#ifdef DEMO
	upgradeManager_->DebugWindow(player_.get());
#endif // DEMO


	if (!upgradeManager_->GetLevelUpFlag())
	{

		// プレイヤー
		player_->Update();

		// 追従カメラ
		followCamera_->Update();

		//mainCamera->transform = followCamera_->camera_.transform;

		enemyManager_->Update();

		// 敵が死んだときに出てくるので敵の更新の後
		// 経験値を更新
		expManager_->Update();

		// 経験値が更新された後かと思ったけど別にプレイヤーの更新が終わった後ならどこでもいい
		level_->Update(player_->GetWorldTransform()->translation);

	}
	else
	{
		upgradeManager_->Update(player_.get());
	}

}

void TItleScene::StartJustSlash()
{
	time_ = 0.0f;
	LWP::Info::SetDeltaTimeMultiply(0.1f);
	followCamera_->StartSlash();
}

void TItleScene::EndJustSlash()
{
	LWP::Info::SetDeltaTimeMultiply(1.0f);
	followCamera_->EndSlash();
}
