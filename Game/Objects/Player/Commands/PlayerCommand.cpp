#include "PlayerCommand.h"

#include "../Player.h"

void PlayerMoveFrontCommand::Exec(Player& player)
{
	player.MoveFront();
}

void PlayerMoveBackCommand::Exec(Player& player)
{
	player.MoveBack();
}

void PlayerMoveLeftCommand::Exec(Player& player)
{
	player.MoveLeft();
}

void PlayerMoveRightCommand::Exec(Player& player)
{
	player.MoveRight();
}

void PlayerSlashCommand::Exec(Player& player)
{
	player.Slash();
}
