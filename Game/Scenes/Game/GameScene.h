#pragma once
#include <algorithm>

#include <scene/IScene.h>

#pragma region GameInclude
#include "Game/Objects/GameTimer/GameTimer.h"
#include "Game/Objects/FollowCamera/FollowCamera.h"
#include "Game/Objects/Player/Player.h"
#include "Game/Objects/Enemy/EnemyManager.h"
#include "Game/Objects/Experience/ExpManager.h"
#include "Game/Objects/Upgrade/UpgradeManager.h"
#pragma endregion

#include "../../SceneTransition/SceneTransition.h"

class GameScene final
	: public IScene
{
public:
	GameScene() = default;	// コンストラクタは自由に定義可能
	~GameScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

public: //*** 関数群 ***//

	// ジャスト抜刀開始
	void StartJustSlash();

	// ジャスト抜刀終了
	void EndJustSlash();

private: //*** 変数群 ***//

	// ゲーム内タイマー
	GameTimer* gameTimer_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<EnemyManager> enemyManager_;

	// 経験値マネージャー
	std::unique_ptr<ExpManager> expManager_;

	// プレイヤー
	std::unique_ptr<Player> player_;

	// レベルアップ機能
	//std::unique_ptr<Level> level_;

	// ジャストからの時間
	float time_ = 0.0f;

	// スローモーションになる時間
	float cTIMESLOW_ = 0.5f;

	// アップグレードの表示
	//std::unique_ptr<UpgradeScreen> scUpgrade_;
	std::unique_ptr<L::UpgradeManager> upgradeManager_;
	// 地面
	lwp::Mesh ground;
	// 天球
	lwp::Mesh skydome;
	lwp::Vector3 scale = { 1.0f,1.0f,1.0f };
	// 太陽
	lwp::PointLight sun_;

	// チュートリアル用Sprite
	LWP::Primitive::Sprite tutorialSprite1;
	LWP::Primitive::Sprite tutorialSprite2;
	// タイマーの後ろのスプライト
	LWP::Primitive::Sprite backSprite_;

	// シーン遷移
	std::unique_ptr<SceneTransition> sceneTransition_;
	//BGM
public:
	float Lerp(const float& v1, const float& v2, float t) {
		float result = v1 + (v2 - v1) * t;
		return result;
	}
private:

	//SE
	bool IsSceneChangeEnd = false;
	std::unique_ptr<LWP::Resource::Audio> BGM;
	float BGMvolume = 0.2f;
	float BGMt = 0.0f;
	std::unique_ptr<LWP::Resource::Audio> atack;
	std::unique_ptr<LWP::Resource::Audio> PlayerDead;
	std::unique_ptr<LWP::Resource::Audio> PlayerAlive;
	std::unique_ptr<LWP::Resource::Audio> EnemyDamege;
	std::unique_ptr<LWP::Resource::Audio> EnemyDead;

	float UVscale = 1.0f;
};