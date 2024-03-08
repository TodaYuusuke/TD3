#pragma once
#include <Adapter.h>

struct MotionWork {
	lwp::Vector3 targetpoint;//目標地点
	float t;//媒介変数
	float speed;//媒介変数の増えるスピード
	bool flag = false;//再生のフラグ
};

class IEnemy
{
public:
	// デストラクタ
	virtual ~IEnemy() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Move(LWP::Math::Vector3 MoveVec) = 0;
	virtual void Attack() = 0;

public: //*** ゲッターセッター ***//

	bool GetIsActive() const { return isActive_; }

protected:
	std::vector<LWP::Primitive::IPrimitive*> models_;

	// 敵の当たり判定
	lwp::Collider::AABB* collider_ = nullptr;
	bool isActive_ = false;

};