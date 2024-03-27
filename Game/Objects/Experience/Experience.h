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
	//static Experience* Create(const lwp::Vector3& pos);

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
	lwp::Mesh* model_ = nullptr;

	// 生成されてから動かないコライダー
	lwp::Collider::Sphere* collider_ = nullptr;

};