#pragma once

#include <Adapter.h>

#include "UpGradeBack/UpgradeBackPanel.h"
#include "UpGradeItem/UpgradeItemPanel.h"

class UpgradePanels final
{
public: //*** パブリック関数 ***//

	UpgradePanels() = default;
	~UpgradePanels();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// アップグレード
	void Start();

	// 終了
	void End();

private: //*** プライベート変数 ***//

	// 背景
	std::unique_ptr<UpgradeBackPanel> backPanel_;

	std::unique_ptr<UpgradeItemPanel> itemPanel0_;
	std::unique_ptr<UpgradeItemPanel> itemPanel1_;
	std::unique_ptr<UpgradeItemPanel> itemPanel2_;

};

