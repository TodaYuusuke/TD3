#pragma once

#include "Adapter.h"

#include <array>
#include <optional>

#include "Game/Objects/Upgrade/IUpgrade.h"
#include "../FollowCamera/FollowCamera.h"

#include "Level/Level.h"
#include "Status/IStatus.h"
#include "Game/Objects/Player/Weapon/Weapon.h"
#include "Game/Objects/Player/SlashPanel.h"
#include "Game/Objects/Player/Config/PlayerConfing.h"
#include "Game/Objects/Player/Parameter/PlayerParametaer.h"
#include "Game/Objects/Player/HP/PlayerHP.h"

#pragma region

#include "Game/Objects/Upgrade/Function/Pursuit/Pursuit.h"
#include "Game/Objects/Upgrade/Function/EXLife/EXLife.h"

#pragma endregion アップグレード

// 前方宣言
class GameScene;

class Player final
{
private: //*** サブクラス ***//

#pragma region SubClass

#pragma region Behavior

	//*** 各 Behavior で使う情報 ***//

	// 共通データ
	// まったくもって参照できないからつかわん
	// 絶対に継承した先のインスタンスを使う
	struct BaseData
	{
		//float cBASETIME = 0.0f;	// 状態の基本時間 : 基本となる秒数
		float maxTime_ = 0.0f;	// 状態の最大時間 : 条件で抜けるときもあるかもしれない
	};

	struct RootData : public BaseData
	{
	};

	struct MoveData : public BaseData
	{
	};

	// 居合攻撃で使うデータ
	struct SlashData : public BaseData
	{
		lwp::Vector3 vector_ = { 0.0f,0.0f,1.0f };		// 向かう方向
		uint32_t relationSlash_;	// 連続居合回数
		uint32_t maxRelation_;		// 最大居合回数(実際の数)(増減しちゃう) 
	};


	// 後隙で使うデータ
	struct MomentData : public BaseData
	{
		uint32_t relationSlash_;	// 連続居合回数 : 二回以降+1
	};

	// 被弾
	struct DamageData : public BaseData
	{

	};

#pragma endregion

	//*** フラグ管理 ***//

	// フラグをまとめた構造体
	struct Flags
	{
		bool isJustSlashing_ = false;	// 今ジャスト抜刀中か
		bool isInvincible_ = false;		// 無敵時間中か
		bool isDamage_ = false;			// ダメージを受けたか
		bool isInputMove_ = false;		// 移動入力があったか
		bool isInputSlash_ = false;		// 攻撃入力があったか
		bool isAlive_ = true;			// プレイヤーが生きているか

	};

	//*** 当たり判定 ***//

	// 当たり判定をオンオフするための変数
	struct Colliders
	{
		// プレイヤー自身の当たり判定
		lwp::Collider::AABB player_;
		// 武器の当たり判定
		lwp::Collider::Capsule weapon_;
		// ジャスト抜刀したいときの大きめの判定
		lwp::Collider::Capsule justSlash_;
	};

#pragma endregion

public: //*** パブリック関数 ***//

	// コンストラクタ
	Player() = default;
	// デストラクタ
	~Player() {
		delete pursuit_;
		delete eXLife_;
	};

	// 初期化
	void Initialize();
	// 更新
	void Update();

	// 体力を回復する
	void IncreaseHP();
	// 体力が減少する
	void DecreaseHP();

	/// <summary>
	/// アップグレードを適応するための関数
	/// <para>UpgradeManagerがアップグレードの選択確定時に呼び出します</para>
	/// </summary>
	/// <param name="para">レベルアップによって変わったパラメーター</param>
	void ApplyUpgrade(const UpgradeParameter& para);

	// クリアアニメ
	bool ClearAnime();

	// ゲームオーバアニメ
	bool GameOverAnime();

public:	//*** セッター,ゲッター ***//

	PlayerParameter* GetPlayerParameter() { return &parameter_; }
	lwp::TransformEuler* GetWorldTransform() { return &demoModel_.transform; }
	bool GetIsJustSlashing() { return flag_.isJustSlashing_; }
	bool GetIsSlash() { return behavior_ == IStatus::Behavior::Slash; }
	bool GetIsEnemyKnockBack() { return isEnemyKnockBack_; }

	void SetCameraPointer(FollowCamera* p) { pCamera_ = p; }
	void SetScene(GameScene* p) { pScene_ = p; }
	// ノックバック判定開始
	void StartEnemyKnockBack() { isEnemyKnockBack_ = true; }

	// 状態を外部から設定する
	void RegistStatus(IStatus::Behavior request);
	// Vector3 をカメラ方向に変える
	lwp::Vector3 GetVectorTranspose(const lwp::Vector3& vec);
	// フラグ
	bool GetPursuitFlag()const { return pursuitFlag; }
	// 
	Pursuit* GetPursuit()const { return pursuit_; }
	// フラグ
	bool GeteXLifeFlag()const { return eXLifeFlag; }
	// 
	EXLife* GeteXLife()const { return eXLife_; }

private: //*** プライベート関数 ***//

#pragma region PrivateFunc

private: //*** Behavior 管理に使う関数 ***//

	//*** 初期化系 ***//

#pragma region CreateCollision

	// 当たり判定の作成
	void CreateCollisions();
	// プレイヤー
	void CreatePlayerCollision();
	// 武器
	void CreateWeaponCollision();

#pragma endregion

#pragma region Effect
	/// <summary>
	/// 静的変数の初期化
	/// </summary>
	void InitStaticVariable();
#pragma endregion

	//*** 更新系 ***//

#pragma region OnCollision

	// プレイヤーの OnCollision
	void OnCollisionPlayer(lwp::Collider::HitData& data);
	// 武器の OnCollision
	void OnCollisionWeapon(lwp::Collider::HitData& data);

#pragma endregion

	/// <summary>
	/// 移動操作入力を受け取る
	/// </summary>
	void CheckInputMove();

	/// <summary>
	/// 攻撃操作入力を受け取る
	/// </summary>
	void CheckInputSlash();

	// 受け付けた入力を判別して実際の行動に反映する
	void CheckBehavior();

#pragma region DebugFunc

	// デバッグ表示
	void DebugWindow();
	// ビヘイビア表示
	void DebugBehavior();
	// 各コンフィグをいじる
	void DebugSpeeds();
	void DebugTimes();
	void DebugLengths();
	void DebugCounts();
	void DebugParcentages();

#pragma endregion

#pragma endregion

	// ダメージを受けた時のリアクション
	void DamageEffect();

public: //*** プライベート変数 ***//

#pragma region PrivateVar

	//*** 外部から設定する変数 ***//

	// 情報
	PlayerConfig config_;

	//*** 各状態毎のデータ ***//

	// 固定されているデータを外部から取得
	RootData rootData_;
	MoveData moveData_;
	SlashData slashData_;
	MomentData momentData_;
	DamageData damageData_;


	//*** プログラム内だけど外部のやつ ***//
	// カメラ
	FollowCamera* pCamera_ = nullptr;

	// 今のシーン
	GameScene* pScene_ = nullptr;

	// アップグレードで作成されたパラメータを反映
	PlayerParameter parameter_;


	//*** 計算に使う ***//

	// 入力したコマンドを一括で管理する
	std::list<IStatus::Behavior> commands_;

	// プレイヤーのモデル
	LWP::Primitive::Mesh demoModel_;
	// 武器
	std::unique_ptr<Weapon> weapon_;

	// レベルアップ機能
	std::unique_ptr<Level> level_;

	MotionWork ClearYUpMotion = {
		.t = 0.0f,
		.speed = 1.0f,
		.flag = false,
	};
	MotionWork ClearZUpMotion = {
		.t = 0.0f,
		.speed = 1.0f,
		.flag = false,
	};
	const float kClearMotionEnd = 30.0f;
	// ゲームオーバーのアニメーション
	LWP::Resource::Motion gameOverMotion_;
	bool isGameOver_;
	int gameOverFrame_;

	// 現在の状態
	IStatus::Behavior behavior_ = IStatus::Behavior::Root;
	// 状態の予約
	std::optional<IStatus::Behavior> reqBehavior_ = std::nullopt;

	// 次に移動する方向
	// これは通常移動くらいでしか使わない
	LWP::Math::Vector3 destinate_ = { 0.0f,0.0f,1.0f };

	// 秒数
	float t = 0.0f;
	// イージング
	float easeT_ = 0.0f;
	// 無敵時間まで加算
	float invincibleTime_ = 0.0f;
	// 全体の無敵時間
	float maxInvincibleTime_ = 0.0f;


	// 居合攻撃を描画
	std::unique_ptr<SlashPanel> slashPanel_;

	// 当たり判定をオンオフするための変数
	Colliders colliders_;

	// フラグ
	Flags flag_;

	// 状態毎に使うクラスをまとめている
	std::vector<IStatus*> statuses_;

	// 今の状態クラス
	IStatus* currStatus_ = nullptr;


	// 敵をノックバックするフラグ
	bool isEnemyKnockBack_;


	// スラッシュ攻撃の土飛沫
	std::function<void(int, lwp::Vector3)> soilSplashEffect_;
	// 死ぬときのエフェクト
	static std::function<void(int, lwp::Vector3)> deadEffect_;

#pragma endregion

private: //*** プライベート関数 ***//

private: //*** アップデート関連クラス ***//
	Pursuit* pursuit_;
	// Pursuitを管理するフラグ
	bool pursuitFlag = false;
	EXLife* eXLife_;
	// Pursuitを管理するフラグ
	bool eXLifeFlag = false;

	float Lerp(const float& v1, const float& v2, float t) {
		float result = v1 + (v2 - v1) * t;
		return result;
	}

	//光の柱
	LWP::Primitive::Billboard2D lightPillar_;
	LWP::Resource::Motion lightPillarMotion_;

public: //*** 音 ***//

	void SetSE(std::vector<LWP::Resource::Audio*> input) { audio = input; };
private:
	std::vector<LWP::Resource::Audio*> audio;
	float soundVolume = 1.0f;
};