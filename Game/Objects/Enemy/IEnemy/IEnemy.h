#pragma once
#include <Adapter.h>
class IEnemy
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Move(LWP::Math::Vector3 MoveVec) = 0;

protected:

	std::vector<LWP::Primitive::IPrimitive*>models_;

};