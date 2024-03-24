#pragma once
#include "Adapter.h"

typedef void (*pfunc)();
class Player;
class Upgrade
{
public:
	Upgrade() = default;
	~Upgrade() = default;

	void Init();
	//void Update();

	pfunc SelectUpgrade();
	void RollUpgrade();

	void SetPlayer(Player* player) { player_ = player; };
	
	std::vector<pfunc> pFunc;
	std::map<int, bool> selectedNum;
	std::map<int, bool> checkUseNum;
	int select1;
	int select2;
	int select3;

private:
	Player* player_;
};

	void DrawOne();
	void DrawTwo();
	void DrawThree();
	void DrawFour();
	void DrawFive();
	void DrawSix();