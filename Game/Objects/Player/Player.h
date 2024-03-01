#pragma once

#include "Adapter.h"

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
		//Attack,		// 居合攻撃

		_COUNT,		// 状態最大数
	};

	//*** 各 Behavior で使う情報 ***//

	struct IData
	{

	};

	struct RootData : public IData
	{

	};

	struct MoveData : public IData
	{

	};

public: //*** パブリック関数 ***//

	Player() {}
	~Player();

	void Initialize();
	// 更新
	void Update();

	// セッターゲッター

	lwp::WorldTransform* GetWorldTransform() { return &world_; }
	void SetCameraPointer(const lwp::Camera* p) { camera_ = p; }

private: //*** プライベート関数 ***//

	// プレイヤーの操作を受け付ける
	void UpdateInput();


public: //*** コマンド操作で呼び出される関数 ***//

	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();

private: //*** Behavior 管理に使う関数 ***//

	void UpdateRoot();
	void UpdateMove();


private: //*** プライベート変数 ***//


private: //*** 外部から設定する変数 ***//

	float cPLAYERSPEED_ = 2.0f;


	// プログラム内だけど外部のやつ
	const LWP::Object::Camera* camera_ = nullptr;



private: //*** 計算に使う ***//

	// プレイヤーのインプットを処理する
	PlayerInput* pInput_ = nullptr;
	// 受け取るコマンド
	// 複数入力に対応させたい
	IPlayerCommand* pCommand_ = nullptr;

	// プレイヤーのモデル
	LWP::Primitive::Mesh* demoModel;

	// ワールド座標
	LWP::Object::WorldTransform world_;

	// 現在の状態
	Behavior behavior_ = Behavior::Root;
	// 状態の予約
	std::optional<Behavior> reqBehavior_ = std::nullopt;

	// 次に移動する速度
	LWP::Math::Vector3 destinate_;

};

