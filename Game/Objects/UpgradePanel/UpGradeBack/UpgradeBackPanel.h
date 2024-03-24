#pragma once

#include "Game/Objects/UpgradePanel/BaseUpgradePanel.h"

class UpgradeBackPanel : public BaseUpgradePanel
{
public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	UpgradeBackPanel() = default;
	~UpgradeBackPanel() override;

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

private: //*** プライベート関数 ***//




#ifdef DEMO

	void DebugWindow() override;

#endif // DEMO

private: //*** プライベート変数 ***//
};

