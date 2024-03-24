#pragma once

#include "Adapter.h"

#include "Game/Objects/UIPanel/UIPanel.h"

class BaseUpgradePanel
{
protected:

	int32_t kWINDOW_WIDTH_HALF_ = LWP::Info::GetWindowWidth() / 2;
	int32_t kWINDOW_HEIGHT_HALF_ = LWP::Info::GetWindowHeight() / 2;

public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	BaseUpgradePanel() = default;
	virtual ~BaseUpgradePanel();

	// 初期化
	virtual void Initialize();

	// 更新
	virtual void Update();

public: //*** ゲッターセッター ***//

	void SetIsActive(bool flag) { panel_->SetIsActive(flag); }

protected: //*** プロテクト関数 ***//




#ifdef DEMO

	virtual void DebugWindow();

#endif // DEMO

protected: //*** プロテクト変数 ***//

	UIPanel* panel_ = nullptr;

};

