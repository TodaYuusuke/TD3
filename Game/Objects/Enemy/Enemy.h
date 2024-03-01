#pragma once
#include <Adapter.h>
class Enemy
{
public:
	void Initialize();
	void Update();

private:
	void Move(LWP::Math::Vector3 MoveVec);

	LWP::Object::WorldTransform world_;

	std::vector<LWP::Primitive::Mesh*>models_;

};