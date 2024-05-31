#include "GameScene.h"

#include "Game/Scenes/Clear/ClearScene.h"
#include "Game/Scenes/GameOver/GameOverScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void GameScene::Initialize()
{
	Info::ChangeShowDebugGUI();

	//mainCamera.pp.use = true;
	//mainCamera.pp.vignetting.use = true;
	//mainCamera.pp.CreateShaderFile();

	// タイマー
	gameTimer_ = GameTimer::GetInstance();
	gameTimer_->Initialize();

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetScene(this);
	atack = std::make_unique<LWP::Resource::Audio>();
	atack->Load("Attack/patternA.mp3");
	PlayerDead = std::make_unique<LWP::Resource::Audio>();
	PlayerDead->Load("Player/Dead.mp3");
	PlayerAlive = std::make_unique<LWP::Resource::Audio>();
	PlayerAlive->Load("Attack/patternA.mp3");
	std::vector<LWP::Resource::Audio*> PlayeraudioSet{ atack.get(),PlayerDead.get(),PlayerAlive.get() };
	player_->SetSE(PlayeraudioSet);
	// 天球
	skydome.LoadFile("skydome/skydome.obj");
	skydome.transform.scale = { 0.5f,0.5f, 0.5f };
	skydome.material.enableLighting = false;

	// 地面
	ground.LoadFile("ground/ground.obj");
	ground.transform.translation.y = -0.5f;
	ground.transform.scale = { 10.0f,0.1f, 10.0f };
	ground.name = "Ground";
	ground.material.enableLighting = true;
	ground.material.uvTransform.scale = { 20,20,0 };

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetCameraAddress(&mainCamera);
	followCamera_->SetTarget(player_->GetWorldTransform());
	followCamera_->SetPlayer(player_.get());
	// カメラを少し上に上げる
	player_->SetCameraPointer(followCamera_.get());

	// 経験値タンク
	expManager_ = std::make_unique<ExpManager>();
	expManager_->Initialize();

	// エネミーマネージャ
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->SetCamera(followCamera_.get());
	EnemyDamege = std::make_unique<LWP::Resource::Audio>();
	EnemyDamege->Load("Slash/patternA.mp3");
	EnemyDead = std::make_unique<LWP::Resource::Audio>();
	EnemyDead->Load("Enemy/Dead.mp3");
	// 経験値マネージャーをエネミーマネージャーに設定
	enemyManager_->SetExpManager(expManager_.get());
	std::vector<LWP::Resource::Audio*> EnemyaudioSet{ EnemyDamege.get(),EnemyDead.get() };
	enemyManager_->SetSE(EnemyaudioSet);
	enemyManager_->Init();

	// アップグレード
	upgradeManager_ = std::make_unique<L::UpgradeManager>();
	upgradeManager_->Init(GetMainRenderCamera());

	tutorialSprite1.texture = Resource::LoadTexture("Text/tutorial1.png");
	tutorialSprite1.isUI = true;
	tutorialSprite1.isActive = true;
	tutorialSprite1.transform.translation.y = 120.0f;
	tutorialSprite2.texture = Resource::LoadTexture("Text/tutorial2.png");
	tutorialSprite2.isUI = true;
	tutorialSprite2.isActive = true;
	tutorialSprite2.transform.translation.y = 200.0f;

	backSprite_.texture = Resource::LoadTexture("white.png");
	backSprite_.isUI = true;
	backSprite_.isActive = false;
	backSprite_.anchorPoint = { 0.5f,0.5f };
	backSprite_.transform.scale.x = 1.2f;
	backSprite_.transform.scale.y = 0.5f;
	backSprite_.transform.translation = { 1600.0f,100.f,0.0f };
	backSprite_.commonColor = new Color(0xAAAAAAFF);

	// いったん三分
	gameTimer_->Reset(180);
	// ゲームが始まってから計測開始
	gameTimer_->Start();

	// 点光源
	sun_.transform.translation.y = 30.0f;
	sun_.intensity = 2.0f;
	sun_.radius = 120.0f;
	sun_.decay = 0.58f;

	sceneTransition_ = std::make_unique<SceneTransition>();
	sceneTransition_->Initialize();

	BGM = std::make_unique<LWP::Resource::Audio>();
	BGM->Load("BGM/GameScene.mp3");
	BGMvolume = 0.2f;
	BGM->Play(BGMvolume,255);
	BGMt = 0.0f;
	IsSceneChangeEnd = true;
}

// 更新
void GameScene::Update()
{

	//だんだん音が上がる
	if (BGMt != 1.0f && IsSceneChangeEnd == true) {
		BGMt = (std::min)(BGMt + 0.01f, 1.0f);
		BGMvolume = Lerp(BGMvolume, 0.8f, BGMt);
	}
	else if(IsSceneChangeEnd == true) {
		IsSceneChangeEnd = false;
		BGMt = 0.0f;
	}
	BGM->SetVolume(BGMvolume);

	sceneTransition_->Update();

	// 時間を計測
	// チュートリアルの時は計測しない
	if (!enemyManager_->GetIsTutorial()) {
		gameTimer_->Update();
		tutorialSprite1.isActive = false;
		tutorialSprite2.isActive = false;
		backSprite_.isActive = true;
	}

	// タイマーのカウントが終了したとき
	if (gameTimer_->isEnd_){
		//だんだん音が下がる
		BGMt = (std::min)(BGMt + 0.002f,1.0f);
		BGMvolume = Lerp(BGMvolume, 0.0f, BGMt);

		// プレイヤーが生きているとき
		if (player_->flag_.isAlive_) {
			// クリアしたときの処理
			// 何か演出を出す
			
			if (player_->ClearAnime()) {
				// タイマーを消す
				gameTimer_->isActive_ = false;
				// 描画を消す
				gameTimer_->Update();


				// シーン遷移演出開始
				sceneTransition_->Start();
				if (sceneTransition_->GetIsSceneChange()) {
					BGM->Stop();
					nextSceneFunction = []() {return new ClearScene; };
				}
			}
			return;
		}
		// プレイヤーが死んでいた時
		else
		{
			
			// ゲームオーバーしたときの処理
			if (player_->GameOverAnime()) {
				// タイマーを消す
				gameTimer_->isActive_ = false;

				// 描画を消す
				gameTimer_->Update();

				// シーン遷移演出開始
				sceneTransition_->Start();

				if (sceneTransition_->GetIsSceneChange()) {
					BGM->Stop();
					nextSceneFunction = []() {return new GameOverScene; };
				}
			}
			return;
		}
	}

	// スローを確認
	if (player_->GetIsJustSlashing())
	{
		time_ += lwp::GetDeltaTimeF();
		if (cTIMESLOW_ <= time_)
		{
			EndJustSlash();
		}
	}

	// デバッグ
//#ifdef DEMO
	upgradeManager_->DebugWindow(player_.get());
	//#endif // DEMO

	if (!upgradeManager_->GetLevelUpFlag())
	{
		// プレイヤー
		player_->Update();

		// 追従カメラ
		followCamera_->Update();

		//mainCamera->transform = followCamera_->camera_.transform;

		// アップグレード関係は敵の前に更新させる
		upgradeManager_->Update(player_.get());

		enemyManager_->Update();

		// 敵が死んだときに出てくるので敵の更新の後
		// 経験値を更新
		expManager_->Update();

	}
	else
	{
		// アップグレード選択中
		upgradeManager_->Selecting(player_.get());
	}
}

void GameScene::StartJustSlash()
{
	//time_ = 0.0f;
	//LWP::Info::SetDeltaTimeMultiply(0.1f);
	followCamera_->StartSlash();
}

void GameScene::EndJustSlash()
{
	LWP::Info::SetDeltaTimeMultiply(1.0f);
	followCamera_->EndSlash();
}
