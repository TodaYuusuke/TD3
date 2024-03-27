#pragma once

#include "Adapter.h"

#include "Game/Objects/UIPanel/UIPanel.h"

/// <summary>
/// プレイヤーの居合を可視化する UI 管理クラス
/// </summary>
class SlashPanel
{
private:

	const static int32_t kWINDOW_WIDTH_HALF_ = 1980 / 2;

public:

	// デストラクタ
	~SlashPanel();

	// 初期化
	void Initialize();

	// 更新
	void Update();

public: //*** 外部からセット ***//

	// 一つ消費
	void Slash();
	// 全部リセット
	void Reset();
	// ジャストを検知
	void Just();

private: //*** プライベート変数 ***//

	// 刀アイコン
	std::vector<UIPanel*> katanaPanels_;
	// 鞘アイコン
	std::vector<UIPanel*> sayaPanels_;

	// 添え字
	// 常にプラス一で計算したい
	size_t index_ = 1u;

};

