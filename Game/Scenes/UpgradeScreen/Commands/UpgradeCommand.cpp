#include "UpgradeCommand.h"

#include "../UpgradeScreen.h"

void UpgradeNextCommand::Exec(UpgradeScreen& screen)
{
}

void UpgradeBackCommand::Exec(UpgradeScreen& screen)
{
}

void UpgradeSelectCommand::Exec(UpgradeScreen& screen)
{
	screen.End();
}
