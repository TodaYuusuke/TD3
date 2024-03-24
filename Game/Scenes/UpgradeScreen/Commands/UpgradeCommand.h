#pragma once

class UpgradeScreen;

class IUpgradeCommand
{
public:
	virtual ~IUpgradeCommand() = default;
	virtual void Exec(UpgradeScreen& screen) = 0;
};

// 次へ
class UpgradeNextCommand : public IUpgradeCommand
{
public:
	void Exec(UpgradeScreen& screen) override;
};
// 前へ
class UpgradeBackCommand : public IUpgradeCommand
{
public:
	void Exec(UpgradeScreen& screen) override;
};

// 選択
class UpgradeSelectCommand : public IUpgradeCommand
{
public:
	void Exec(UpgradeScreen& screen) override;
};