#pragma once
#include <Adapter.h>

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
	void DyingAnimation();

public: //*** ゲッターセッター ***//

	bool GetIsActive() const { return isActive_; }

	// 狙う対象をセット(今回は自機をセットする)
	virtual void SetTarget(Player* player) { player_ = player; }
	virtual void SetPosition(lwp::Vector3 pos) { models_[0].transform.translation = pos; }

protected: //*** 継承クラスで呼び出す共通処理 ***//

	virtual void CreateCollider();


protected:
	std::vector<LWP::Primitive::Mesh> models_;

	// 敵の当たり判定
	lwp::Collider::AABB collider_;

	// 
	Player* player_;
	// 攻撃のクールタイム
	int attackWaitTime_;
	// 攻撃開始フラグ
	bool isAttack = false;
	// 死んだときに立つフラグ(アニメーション用)
	bool IsDead_ = false;
	// 死んだ時用アニメ
	MotionWork deadAnime = {
	// 下に沈んでいく速度
	.speed = 0.5f,
	};
	int deadFlame = 0;
	// 生きているかどうか
	bool isActive_ = true;
};