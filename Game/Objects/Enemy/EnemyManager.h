#pragma once
#include "IEnemy/IEnemy.h"
#include "NormalEnemy/NormalEnemy.h"
#include "ShieldEnemy/ShieldEnemy.h"
#include "ArrowEnemy/ArrowEnemy.h"
#include "Boss/DashBoss.h"
#include "Boss/ArrowBoss.h"
#include "Boss/JumpBoss.h"
#include "Game/Objects/Experience/ExpManager.h"
#include "Game/Objects/GameTimer/GameTimer.h"
#include <random>
#include <numbers>

class Player;
class EnemyManager {
public:
	/// 
	/// Default Method
	/// 

	~EnemyManager()
	{
		for (IEnemy* e : enemys_)
		{
			delete e;
		}
	}
	void Init();
	void Update();

	/// 
	/// User Method
	/// 

#pragma region 敵の発生
	// 敵の発生場所をランダムで決める
	void EnemySpawn();
	// 通常
	void NormalEnemySpawn(lwp::Vector3 pos);
	// 盾
	void ShieldEnemySpawn(lwp::Vector3 pos);
	// 弓
	void ArrowEnemySpawn(lwp::Vector3 pos);

	// ボスの発生条件と場所
	void BossSpawn();
	// ダッシュするボス
	void DashBossSpawn(lwp::Vector3 pos);
	// 遠距離ボス
	void ArrowBossSpawn(lwp::Vector3 pos);
	// ジャンプ攻撃するボス
	void JumpBossSpawn(lwp::Vector3 pos);
#pragma endregion

	/// Getter

	// チュートリアルモードになっているかを取得
	bool GetIsTutorial() { return isTutorial_; }

	/// Setter
	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }
	// 経験値マネージャを設定
	void SetExpManager(ExpManager* p) { exp_ = p; }
	// 追従カメラのアドレスを設定
	void SetCamera(FollowCamera* camera) { followCamera_ = camera; }

private:// 定数
	// 敵発生頻度
	const int kSpawnFrequency = 120;
	// ボス発生のクールタイム
	const int kSpawnBossCoolTime = 60;

private:
	// ゲームタイマー
	GameTimer* gameTimer_;

	// 追従カメラのアドレス
	FollowCamera* followCamera_;

	// ボスも含めたすべての敵
	std::list<IEnemy*> enemys_;
	// チュートリアル用の敵
	NormalEnemy* tutorialEnemy_;

	// 自機
	Player* player_;

	// 経験値マネージャー
	ExpManager* exp_;

	// 現在のフレーム
	int currentFrame_ = 0;

	int SpawnNum = 0;

	// チュートリアルフラグ
	bool isTutorial_;

	// ボスのスポーンフラグ
	bool isBossSpawn_;
	// 出現するボスのタイプ
	enum class SpawnBoss {
		NONE,
		DASH,
		ARROW
	};
	int spawnBoss_;
	// ボス発生のクールタイム(既定の時間が過ぎるまでボスが湧かないようにする)
	int spawnBossCoolTime_;

private: //*** プライベート関数 ***//

	void DebugWindow();

	/// <summary>
	/// 経過時点でのステージを取得する
	/// </summary>
	int GetCurrentStage();

public:
	void SetSE(std::vector<LWP::Resource::Audio*> input) { audio = input; };
private:
	std::vector<LWP::Resource::Audio*> audio;
	
};