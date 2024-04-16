#pragma once
#include <Adapter.h>
#include "../../FollowCamera/FollowCamera.h"

struct MotionWork {
	lwp::Vector3 targetpoint;//目標地点
	float t;//媒介変数
	float speed;//媒介変数の増えるスピード
	bool flag = false;//再生のフラグ
};

class Player;
class IEnemy
{
public:
	// デストラクタ
	virtual ~IEnemy() = default;

	void Initialize();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Attack() = 0;


public: //*** ゲッターセッター ***//

	bool GetIsActive() const { return isActive_; }

	// 狙う対象をセット(今回は自機をセットする)
	virtual void SetTarget(Player* player) { player_ = player; }
	virtual void SetPosition(lwp::Vector3 pos) { models_[0].transform.translation = pos; }
	// カメラのアドレスを設定
	virtual void SetCamera(FollowCamera* camera) { followCamera_ = camera; }

protected: //*** 継承クラスで呼び出す共通処理 ***//

	virtual void CreateCollider();

protected:// 定数
	const LWP::Math::Vector3 kBossSize = { 4,5,4 };

protected:
	std::vector<LWP::Primitive::Mesh> models_;

	// 追従カメラのアドレスを受け取る
	FollowCamera* followCamera_;

	// 敵の当たり判定
	lwp::Collider::AABB collider_;
	bool isActive_ = false;
	// 
	Player* player_;
	// 攻撃のクールタイム
	int attackWaitTime_;
	// 攻撃開始フラグ
	bool isAttack = false;
};