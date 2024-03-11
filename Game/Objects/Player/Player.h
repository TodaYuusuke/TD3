#pragma once

#include "Adapter.h"

#include <array>
#include <optional>

#include "Commands/PlayerInput.h"
#include "Game/Objects/Player/Weapon/Weapon.h"
#include "Game/Objects/Player/SlashPanel.h"

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

		_COUNT,		// 状態最大数 : 使用禁止
	};

	/// <summary>
	/// プレイヤー行動
	/// 実際にコマンドとして入力したものが対象
	/// 後隙への状態遷移は含まない
	/// コマンドを溜める
	/// 移動がどうするか迷う
	/// 一応下にあるほど優先度が高いと思いたい
	/// </summary>
	//enum class Command : size_t
	//{
	//	toMoveF = 0u,	// 前移動
	//	toMoveB,		// 後ろ移動
	//	toMoveL,		// 左移動
	//	toMoveR,		// 右移動
	//	toSlash,		// 居合攻撃

	//	_COUNT,		// 状態最大数 : 使用禁止
	//};



	//*** 各 Behavior で使う情報 ***//

	// 共通データ
	// まったくもって参照できないからつかわん
	// 絶対に継承した先のインスタンスを使う
	struct BaseData
	{
		uint32_t frame_ = 0u;		// 状態に入ってから経過したフレーム
		uint32_t cBASEFRAME = 0u;	// 状態の基本フレーム : 基本となるフレーム数
		uint32_t maxFrame_ = 0u;	// 状態の最大フレーム : 条件で抜けるときもあるかもしれない
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
		uint32_t MaxRelation_;		// 最大居合回数 : Max
	};


	// 後隙で使うデータ
	struct MomentData : public BaseData
	{
		uint32_t relationSlash_;	// 連続居合回数 : 二回以降+1
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

public:	//*** セッター,ゲッター ***//

	lwp::WorldTransform* GetWorldTransform() { return &demoModel_->transform; }
	void SetCameraPointer(const lwp::Camera* p) { pCamera_ = p; }
	void SetScene(IScene* p) { pScene_ = p; }

public: //*** コマンド操作で呼び出される関数 ***//

	// ここではまだ実際に行動に移さない
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void Slash();

private: //*** Behavior 管理に使う関数 ***//

	void UpdateRoot();
	void UpdateMove();
	void UpdateSlash();
	void UpdateMoment();

private: //*** プライベート関数 ***//

	// データの情報を取得する
	void InitDatas();
	// 状態の値を取得
	RootData* InitRootData();
	MoveData* InitMoveData();
	SlashData* InitSlashData();
	MomentData* InitMomentData();

	// 当たり判定の作成
	void CreateCollision();

	// プレイヤーの操作を受け付ける
	void UpdateInput();
	// 受け付けた入力を判別して実際の行動に反映する
	void CheckBehavior();

	void DebugWindow();

private: //*** プライベート変数 ***//


	//*** 外部から設定する変数 ***//

	// プレイヤーの通常移動
	// 移動する距離
	float cSPEEDMOVE_ = 4.0f;

	// プレイヤーの居合
	// 移動する距離
	float cSPEEDSLASH_ = 40.0f;

	// 居合による後隙の加算分
	uint32_t cFRAMEINCREMENTMOMENT_ = 30u;

	// 各状態毎のデータ
	// 固定されているデータを外部から取得
	//std::array<std::unique_ptr<BaseData>, static_cast<size_t>(Behavior::_COUNT)> behaviorDatas_;
	std::unique_ptr<RootData> rootData_;
	std::unique_ptr<MoveData> moveData_;
	std::unique_ptr<SlashData> slashData_;
	std::unique_ptr<MomentData> momentData_;

	// プログラム内だけど外部のやつ
	// カメラ
	const LWP::Object::Camera* pCamera_ = nullptr;

	// 今のシーン
	const IScene* pScene_ = nullptr;

	//*** 計算に使う ***//

	// プレイヤーのインプットを処理する
	PlayerInput* pInput_ = nullptr;
	// 受け取るコマンド
	// 複数入力に対応させたい
	// コマンドを積み重ねる
	std::list<IPlayerCommand*>* pCommands_;

	// プレイヤーのモデル
	LWP::Primitive::Mesh* demoModel_ = nullptr;
	// 武器
	std::unique_ptr<Weapon> weapon_;


	// 現在の状態
	Behavior behavior_ = Behavior::Root;
	// 状態の予約
	std::optional<Behavior> reqBehavior_ = std::nullopt;
	// 現在の状態のデータ
	//BaseData* currentData_ = nullptr;


	// 次に移動する方向
	// これは通常移動くらいでしか使わない
	LWP::Math::Vector3 destinate_ = { 0.0f,0.0f,1.0f };
	// キーまたはスティックでの移動が入力されたか
	bool isInputMove_ = false;


	// 入力したコマンドを一括で管理する
	std::list<Behavior> commands_;
	// 最終的に行動するコマンド
	Behavior* command_ = nullptr;

	// イージング
	float t = 0.0f;

	// 居合攻撃を描画
	std::unique_ptr<SlashPanel> slashPanel_;

	// 当たり判定をオンオフするための変数
	// プレイヤー自身の当たり判定
	lwp::Collider::AABB* playerCollision_ = nullptr;
	// 武器の当たり判定
	lwp::Collider::AABB* weaponCollision_ = nullptr;
	// ジャスト居合したいときの大きめの判定
	lwp::Collider::AABB* justCollision_ = nullptr;
};

