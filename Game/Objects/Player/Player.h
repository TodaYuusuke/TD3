#pragma once

#include "Adapter.h"

#include <array>
#include <optional>

#include "Commands/PlayerInput.h"
#include "Game/Objects/Player/Weapon/Weapon.h"
#include "Game/Objects/Player/SlashPanel.h"
#include "../FollowCamera/FollowCamera.h"

class Player final
{
private:

	//*** Behaivior で管理する ***//

	/// <summary>
	/// プレイヤー行動
	/// </summary>
	enum class Behavior : size_t
	{
		Root = 0u,	// 待機状態
		Move,		// 移動
		Slash,		// 居合攻撃
		Moment,		// とりあえず居合の後隙
		Damage,		// 攻撃を受けた時

		_COUNT,		// 状態最大数 : 使用禁止
	};

	//*** 各 Behavior で使う情報 ***//

	// 共通データ
	// まったくもって参照できないからつかわん
	// 絶対に継承した先のインスタンスを使う
	struct BaseData
	{
		//float time_ = 0.0f;		// 状態に入ってから経過した時間(second)
		float cBASETIME = 0.0f;	// 状態の基本時間 : 基本となる秒数
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
		lwp::Vector3 vector_;		// 向かう方向
		uint32_t relationSlash_;	// 連続居合回数 : 二回以降+1
		uint32_t maxRelation_;		// 最大居合回数 : Max
		uint32_t cMAXRELATION_;
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

	//*** 外部設定の変数管理 ***//

	// 移動距離 : (1 秒間に進む距離)
	struct Speeds
	{
		// プレイヤーの通常移動
		// 移動する距離
		float MOVE_ = 1.0f;
		// プレイヤーの居合
		// 移動する距離
		float SLASH_ = 20.0f;
		// プレイヤーの後隙
		// 移動する距離
		float MOMENT_ = 2.0f;
	};

	// 秒時間
	struct Times
	{
		// 居合による後隙の加算分
		float INCREMENTMOMENT_ = 0.25f;
		// ジャスト居合を取る時間
		float TAKEJUSTSLASH_ = 0.1f;
		// ジャスト居合に加えて無敵時間
		float ADDINCVINCIBLE_ = 0.1f;
	};

	// 半径
	struct Lengths
	{
		// 武器の半径
		float RADIUSWEAPONCOLLISION_ = 1.0f;
		// 居合時の武器の前側への補正
		float CORRECTIONPLUSWEAPON_ = 4.0f;
		// ジャストの半径
		float RADIUSJUSTCOLLISION_ = 1.5f;
		// 敵の攻撃からの有効範囲
		float RANGEJUSTENABLE_ = 2.0f;
	};

	struct Configs
	{
		Speeds Speed_;
		Times Time_;
		Lengths Length_;
	};


	//*** フラグ管理 ***//

	// フラグをまとめた構造体
	struct Flags
	{
		bool isInputMove_ = false;		// 移動入力がされているか
		bool isJustSlashing_ = false;	// 今ジャスト抜刀中か
	};

	//*** 当たり判定 ***//

	// 当たり判定をオンオフするための変数
	struct Colliders
	{
		// プレイヤー自身の当たり判定
		lwp::Collider::AABB* player_ = nullptr;
		// 武器の当たり判定
		lwp::Collider::Capsule* weapon_ = nullptr;
		// ジャスト抜刀したいときの大きめの判定
		lwp::Collider::Capsule* justSlash_ = nullptr;
	};

public: //*** パブリック関数 ***//

	// コンストラクタ
	Player() = default;
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 更新
	void Update();

	// ジャスト終了
	void EndJust();

public:	//*** セッター,ゲッター ***//

	lwp::WorldTransform* GetWorldTransform() { return &demoModel_->transform; }
	void SetCameraPointer(FollowCamera* p) { pCamera_ = p; }
	void SetScene(IScene* p) { pScene_ = p; }

public: //*** コマンド操作で呼び出される関数 ***//

	// ここではまだ実際に行動に移さない
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void Slash();

private: //*** Behavior 管理に使う関数 ***//

	void InitRoot();
	void InitMove();
	void InitSlash();
	void InitMoment();
	void InitDamage();

	void UpdateRoot();
	void UpdateMove();
	void UpdateSlash();
	void UpdateMoment();
	void UpdateDamage();

private: //*** プライベート関数 ***//

	//*** 初期化系 ***//

	// データの情報を取得する
	void InitDatas();
	
	// 設定を初期化
	void InitConfigs();
	// 移動距離
	void InitSpeeds();
	// 時間
	void InitTimes();
	// 長さ
	void InitLengths();

	// 状態の値を取得
	void InitRootData();
	void InitMoveData();
	void InitSlashData();
	void InitMomentData();
	void InitDamageData();

	// 当たり判定の作成
	void CreateCollisions();
	void CreatePlayerCollision();
	void CreateWeaponCollision();
	// ジャスト居合の生成
	void CreateJustCollision();


	//*** 更新系 ***//

	// プレイヤーの OnCollision
	void OnCollisionPlayer(lwp::Collider::HitData& data);
	// 武器の OnCollision
	void OnCollisionWeapon(lwp::Collider::HitData& data);
	// ジャスト抜刀の OnCollision
	void OnCollisionJust(lwp::Collider::HitData& data);

	// プレイヤーの操作を受け付ける
	void UpdateInput();
	// 受け付けた入力を判別して実際の行動に反映する
	void CheckBehavior();

	// デバッグ表示
	void DebugWindow();

private: //*** プライベート変数 ***//


	//*** 外部から設定する変数 ***//

	// プレイヤーの通常移動
	//// 移動する距離
	//float cSPEEDMOVE_ = 1.0f;
	//// プレイヤーの居合
	//// 移動する距離
	//float cSPEEDSLASH_ = 20.0f;
	//// プレイヤーの後隙
	//// 移動する距離
	//float cSPEEDMOMENT_ = 2.0f;

	//// 居合による後隙の加算分
	//float cTIMEINCREMENTMOMENT_ = 0.25f;
	//// ジャスト居合を取る時間
	//float cTIMEJUSTSLASH_ = 0.1f;
	//// ジャスト居合に加えて無敵時間
	//float cTIMEADDINCVINCIBLE_ = 0.1f;

	//// 武器の半径
	//float cRADIUSWEAPONCOLLISION_ = 1.0f;
	//// 居合時の武器の前側への補正
	//float cPLUSWEAPONCORRECTION_ = 4.0f;
	//// ジャストの半径
	//float cRADIUSJUSTCOLLISION_ = 1.5f;
	//// 敵の攻撃からの有効範囲
	//float cRANGEJUSTENABLE_ = 2.0f;

	// 情報
	Configs config_;

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
	IScene* pScene_ = nullptr;


	//*** 計算に使う ***//

	// プレイヤーのインプットを処理する
	PlayerInput* pInput_ = nullptr;
	// 受け取るコマンド
	// 複数入力に対応させたい
	// コマンドを積み重ねる
	std::list<IPlayerCommand*>* pCommands_;
	// 入力したコマンドを一括で管理する
	std::list<Behavior> commands_;
	// 最終的に行動するコマンド
	Behavior* command_ = nullptr;

	// プレイヤーのモデル
	LWP::Primitive::Mesh* demoModel_ = nullptr;
	// 武器
	std::unique_ptr<Weapon> weapon_;

	// 現在の状態
	Behavior behavior_ = Behavior::Root;
	// 状態の予約
	std::optional<Behavior> reqBehavior_ = std::nullopt;

	// 次に移動する方向
	// これは通常移動くらいでしか使わない
	LWP::Math::Vector3 destinate_ = { 0.0f,0.0f,1.0f };
	// キーまたはスティックでの移動が入力されたか
	//bool isInputMove_ = false;

	// 秒数
	float t = 0.0f;
	// イージング
	float easeT_ = 0.0f;

	// 居合攻撃を描画
	std::unique_ptr<SlashPanel> slashPanel_;

	// 当たり判定をオンオフするための変数
	Colliders colliders_;
	// 当たり判定をオンオフするための変数
	// プレイヤー自身の当たり判定
	//lwp::Collider::AABB* playerCollision_ = nullptr;
	// 武器の当たり判定
	//lwp::Collider::Capsule* weaponCollision_ = nullptr;
	// ジャスト抜刀したいときの大きめの判定
	//lwp::Collider::Capsule* justCollision_ = nullptr;

	// ジャスト中か
	//bool isJustSlashing_ = false;


	// フラグ
	Flags flag_;

};

