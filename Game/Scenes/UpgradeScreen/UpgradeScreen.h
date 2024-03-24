#pragma once

#include <Adapter.h>

#include "Game/Objects/UpgradePanel/UpgradePanels.h"
#include "Commands/UpgradeInput.h"
#include "Commands/UpgradeCommand.h"

class UpgradeScreen final
{
public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	UpgradeScreen() = default;
	~UpgradeScreen();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// アップグレードに入る
	// レベルアップした瞬間に呼ぼうと思う
	void Upgrade();

	// アップグレードが終わったら呼びたい
	void End();

public: //*** ゲッターセッター ***//

	bool GetIsUpgrade() { return isUpgrade_; }

private: //*** プライベート関数 ***//


	// 操作を受け付ける
	void UpdateInput();


#ifdef DEMO

	void DebugWindow();

#endif // DEMO

private: //*** プライベート変数 ***//

	// ハンドルインプット
	UpgradeInput* pInput_ = nullptr;
	// コマンド
	std::list<IUpgradeCommand*>* pCommands_ = nullptr;


	// アップグレードの画面
	std::unique_ptr<UpgradePanels> panels_;

	// 今アップグレード中？
	bool isUpgrade_ = false;


};