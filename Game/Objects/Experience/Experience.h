#pragma once

#include <Adapter.h>

class Experience
{
public: //*** パブリック関数 ***//

	// コンストラクタ
	Experience();
	Experience(const lwp::Vector3& pos);
	// デストラクタ
	~Experience();

	// 更新
	void Update();

	// 生成
	static void InitParticle();

private: //*** プライベート関数 ***//

	// 外部から呼び出さなくていい
	// 初期化
	void Initialize(const lwp::Vector3& pos);

	// 当たり判定の作成
	void CreateCollision();

	// プレイヤーと当たった時の処理
	void OnCollision(const lwp::Collider::HitData& data);

public: //*** パブリック変数 ***//
	
	// この実態を消去してもいいか
	bool isDead_ = false;


private: //*** プライベート変数 ***//

	// モデル
	lwp::Mesh model_;

	// 生成されてから動かないコライダー
	lwp::Collider::Sphere collider_;

	// 参照する移動先
	const lwp::Vector3* lvPosition_ = nullptr;

	// いま判定がない時間かを判断
	bool isDisable_ = true;

	// アニメーションする時間
	const float kAnimationTime_ = 0.5f;
	
	// 判定を付与するまでの時間
	const float kToEnableTime_ = 1.0f;

	// サイズ
	const float kSize_ = 0.25f;

	// 加算する変数
	float time_ = 0.0f;

	// モデルの回転力
	float rotateSpeed_ = 0.3f;
	float kRotateGetSpeed_ = 0.7f;


	// パーティクル関係
	static std::function<void(int, lwp::Vector3)> expGetEffect_;

};