#pragma once

class Player;

class IPlayerCommand 
{
public:
	virtual ~IPlayerCommand() = default;
	virtual void Exec(Player& player) = 0;
};

class PlayerMoveFrontCommand : public IPlayerCommand
{
public:
	void Exec(Player& player) override;
};
class PlayerMoveBackCommand : public IPlayerCommand
{
public:
	void Exec(Player& player) override;
};
class PlayerMoveLeftCommand : public IPlayerCommand
{
public:
	void Exec(Player& player) override;
};
class PlayerMoveRightCommand : public IPlayerCommand
{
public:
	void Exec(Player& player) override;
};