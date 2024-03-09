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
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Move(LWP::Math::Vector3 MoveVec) = 0;
	virtual void Attack() = 0;
	// 狙う対象をセット(今回は自機をセットする)
	virtual void SetTarget(Player* player) { player_ = player; }

protected:
	std::vector<LWP::Primitive::IPrimitive*>models_;
	// 
	Player* player_;
};