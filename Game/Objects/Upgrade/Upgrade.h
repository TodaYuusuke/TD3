#pragma once
#include "Adapter.h"

typedef void (*pfunc)();
class UpgradeManager
{
public:
	UpgradeManager() = default;
	~UpgradeManager() = default;

	void Init();
	void Update();

	std::vector<pfunc> pFunc;

private:

};

	void DrawOne();
	void DrawTwo();
	void DrawThree();