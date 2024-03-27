#pragma once

#include "Adapter.h"

#include <array>
#include <optional>

#include "Game/Objects/Player/Weapon/Weapon.h"
#include "Game/Objects/Player/SlashPanel.h"
#include "../FollowCamera/FollowCamera.h"
#include "Status/IStatus.h"

#include "Game/Objects/Upgrade/IUpgrade.h"

// 前方宣言
class TItleScene;

class Player final
{
private: //*** サブクラス ***//

#pragma region SubClass

	//*** Behaivior で管理する ***//

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
		uint32_t relationSlash_;	// 連続居合回数 : 二回以降+1
		uint32_t maxRelation_;		// 最大居合回数 : Max
		//uint32_t cMAXRELATION_;
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

	//*** 外部設定の変数管理 ***//

#pragma region Config

	// 移動距離 : (1 秒間に進む距離)
	struct Speeds
	{
		// プレイヤーの通常移動
		// 移動する距離
		float MOVE_ = 15.0f;
		// プレイヤーの居合
		// 移動する距離
		float SLASH_ = 100.0f;
		// プレイヤーの後隙
		// 移動する距離
		float MOMENT_ = 10.0f;
	};

	// 秒時間
	struct Times
	{
		// 各状態にかかる時間
		float ROOTBASE_ = 0.5f;
		float MOVEBASE_ = 0.1f;
		float SLASHBASE_ = 0.3f;
		float MOMENTBASE_ = 0.5f;
		float DAMAGEBASE_ = 0.5f;

		// 居合による後隙の加算分
		float MOMENTINCREMENT_ = 0.25f;
		// ジャスト居合を取る時間
		float JUSTTAKETIME_ = 0.1f;
		// ジャスト抜刀判定時間に加える無敵時間
		float JUSTINVINCIBLECORRECTION_ = 0.05f;
		// ジャスト抜刀成功時に加える無敵時間
		float JUSTINVINCIBLEADD_ = 0.1f;
		// 被ダメージ時の無敵時間
		float DAMAGEINVINCIBLE_ = 2.0f;
	};

	// 半径
	struct Lengths
	{
		// 武器の半径
		float WEAPONCOLLISIONRADIUS_ = 1.5f;
		// 居合時の武器の前側への補正
		float WEAPONPLUSCORRECTION_ = 4.0f;
		// ジャストの半径
		float JUSTCOLLISIONRADIUS_ = 1.5f;
		// 敵の攻撃からの有効範囲
		//float JUSTENABLERANGE_ = 2.0f;
	};

	// カウント系
	struct Counts
	{
		uint32_t SLASHRELATIONMAX_ = 3u;
	};

	// 割合系
	struct Parcentages
	{
		// 居合攻撃からジャスト判定が取れる距離の割合
		float JUSTENABLE_ = 0.2f;
	};

	struct Configs
	{
		Speeds Speed_;
		Times Time_;
		Lengths Length_;
		Counts Count_;
		Parcentages Par_;
	};

	//*** プレイヤーが持つアップグレードのパラメータ ***//

	/// <summary>
	/// 最終的なパラメータをまとめている
	/// <para>適用する関数内でプラスもマイナスもするデータ</para>
	/// </summary>
	struct PlayerParameter
	{
		// 攻撃力差分
		float power_ = 0.0f;
		// 移動速度差分
		float moveSpeed = 0.0f;
		// 移動速度差分
		float slashSpeed = 0.0f;
		// 移動速度差分
		float momentSpeed = 0.0f;
		// 居合回数差分
		int slashNum = 0;
	};

#pragma endregion

	//*** フラグ管理 ***//

	// フラグをまとめた構造体
	struct Flags
	{
		bool isInputMove_ = false;		// 移動入力がされているか
		bool isJustSlashing_ = false;	// 今ジャスト抜刀中か
		bool isInvincible_ = false;		// 無敵時間中か
		bool isDamage_ = false;			// ダメージを受けたか
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

#pragma endregion

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
	void StartJust();
	// ジャスト終了
	void EndJust();


	/// <summary>
	/// アップグレードを適応するための関数
	/// <para>UpgradeManagerがアップグレードの選択確定時に呼び出します</para>
	/// </summary>
	/// <param name="para">レベルアップによって変わったパラメーター</param>
	void ApplyUpgrade(const UpgradeParameter& para);

public:	//*** セッター,ゲッター ***//

	lwp::WorldTransform* GetWorldTransform() { return &demoModel_->transform; }
	bool GetIsJustSlashing() { return isJustSlashing_; }
	bool GetIsSlash() { return isSlash_; }
	void SetCameraPointer(FollowCamera* p) { pCamera_ = p; }
	void SetScene(TItleScene* p) { pScene_ = p; }
	// 状態を外部から設定する
	void RegistStatus(IStatus::Behavior request);
	// Vector3 をカメラ方向に変える
	lwp::Vector3 GetVectorTranspose(const lwp::Vector3& vec);

private: //*** プライベート関数 ***//

#pragma region PrivateFunc

private: //*** Behavior 管理に使う関数 ***//

#pragma region BehaviorFunc

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

#pragma endregion

	//*** 初期化系 ***//

#pragma region DataFunc

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

#pragma endregion

#pragma region CreateCollision

	// 当たり判定の作成
	void CreateCollisions();
	// プレイヤー
	void CreatePlayerCollision();
	// 武器
	void CreateWeaponCollision();
	// ジャスト居合の生成
	void CreateJustCollision();

#pragma endregion


	//*** 更新系 ***//

#pragma region OnCollision

	// プレイヤーの OnCollision
	void OnCollisionPlayer(lwp::Collider::HitData& data);
	// 武器の OnCollision
	void OnCollisionWeapon(lwp::Collider::HitData& data);
	// ジャスト抜刀の OnCollision
	void OnCollisionJust(lwp::Collider::HitData& data);

#pragma endregion

	// 受け付けた入力を判別して実際の行動に反映する
	void CheckBehavior();

#pragma region DebugFunc

	// デバッグ表示
	void DebugWindow();
	// 各コンフィグをいじる
	void DebugSpeeds();
	void DebugTimes();
	void DebugLengths();
	void DebugCounts();
	void DebugParcentages();

#pragma endregion

#pragma endregion

public: //*** プライベート変数 ***//

#pragma region PrivateVar

	//*** 外部から設定する変数 ***//

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
	TItleScene* pScene_ = nullptr;

	// アップグレードで作成されたパラメータを反映
	PlayerParameter parameter_;


	//*** 計算に使う ***//

	// 入力したコマンドを一括で管理する
	std::list<IStatus::Behavior> commands_;

	// プレイヤーのモデル
	LWP::Primitive::Mesh* demoModel_ = nullptr;
	// 武器
	std::unique_ptr<Weapon> weapon_;

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

	// 移動入力があったか
	bool isInputMove_ = false;

	// 攻撃入力があったか
	bool isInputSlash_ = false;

#pragma endregion

private: //*** プライベート関数 ***//

	void ResetParameter();

};

