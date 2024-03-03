#pragma once

#include "Adapter.h"

#include <array>
#include <optional>

#include "Commands/PlayerInput.h"

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
	enum class Command : size_t
	{
		toMoveF = 0u,	// 前移動
		toMoveB,		// 後ろ移動
		toMoveL,		// 左移動
		toMoveR,		// 右移動
		toSlash,		// 居合攻撃

		_COUNT,		// 状態最大数 : 使用禁止
	};



	//*** 各 Behavior で使う情報 ***//

	// 共通データ
	struct BaseData
	{
		uint32_t frame_ = 0u;		// 状態に入ってから経過したフレーム
		uint32_t cMAXFRAME = 0u;	// 状態の最大フレーム : 条件で抜けるときもあるかもしれない
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
		// 向かう方向
		lwp::Vector3 vector_;
	};


	// 後隙で使うデータ
	struct MomentData : public BaseData
	{
	};

public: //*** パブリック関数 ***//

	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 更新
	void Update();

public:	//*** セッター,ゲッター ***//

	lwp::WorldTransform* GetWorldTransform() { return &world_; }
	void SetCameraPointer(const lwp::Camera* p) { camera_ = p; }

public: //*** コマンド操作で呼び出される関数 ***//

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
	BaseData* InitRootData();
	BaseData* InitMoveData();
	BaseData* InitSlashData();
	BaseData* InitMomentData();

	// プレイヤーの操作を受け付ける
	void UpdateInput();



private: //*** プライベート変数 ***//


	//*** 外部から設定する変数 ***//

	// プレイヤーの通常移動
	float cMOVESPEED_ = 2.0f;

	// プレイヤーの通常移動
	float cSLASHSPEED_ = 40.0f;

	// 各状態毎のデータ
	// 固定されているデータを外部から取得
	std::array<std::unique_ptr<BaseData>, static_cast<size_t>(Behavior::_COUNT)> behaviorDatas_;


	// プログラム内だけど外部のやつ
	const LWP::Object::Camera* camera_ = nullptr;



	//*** 計算に使う ***//

	// プレイヤーのインプットを処理する
	PlayerInput* pInput_ = nullptr;
	// 受け取るコマンド
	// 複数入力に対応させたい
	std::list<IPlayerCommand*> pCommand_;
	// コマンドを積み重ねる
	std::list<IPlayerCommand*>* pCommands_;

	// プレイヤーのモデル
	LWP::Primitive::Mesh* demoModel = nullptr;

	// ワールド座標
	LWP::Object::WorldTransform world_;

	// 現在の状態
	Behavior behavior_ = Behavior::Root;
	// 状態の予約
	std::optional<Behavior> reqBehavior_ = std::nullopt;
	// 現在の状態のデータ
	BaseData* currentData_ = nullptr;


	// 次に移動する方向
	// これは通常移動くらいでしか使わない
	LWP::Math::Vector3 destinate_;

	// 入力したコマンドを一括で管理する
	std::list<Command> commands_;
	// 最終的に行動するコマンド
	Command* command_ = nullptr;


};

