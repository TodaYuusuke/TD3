#include "IStatus.h"

#include "../Player.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Math;

void IStatus::Init(Player* p)
{
	assert(p);
	player_ = p;

	motions_.clear();
	// アニメーションの作成
	CreateMotions();
}

void IStatus::Reset()
{
	for (size_t i = 0; i < motions_.size(); i++)
	{
		motions_[i].Start();
	}
}

void IStatus::Update()
{

}
