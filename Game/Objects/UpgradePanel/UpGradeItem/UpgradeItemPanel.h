#pragma once

#include "Game/Objects/UpgradePanel/BaseUpgradePanel.h"

class UpgradeItemPanel : public BaseUpgradePanel
{
public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	UpgradeItemPanel() = default;
	~UpgradeItemPanel() override;

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

public: //*** ゲッターセッター ***//

	void SetPosition(const lwp::Vector2& pos) { panel_->SetPosition(pos); }
	void SetPositionX(float x) { panel_->SetPositionX(x); }
	void SetPositionY(float y) { panel_->SetPositionY(y); }

private: //*** プライベート関数 ***//




#ifdef DEMO

	void DebugWindow() override;

#endif // DEMO

private: //*** プライベート変数 ***//
};