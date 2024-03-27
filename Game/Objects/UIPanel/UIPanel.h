#pragma once

#include "Adapter.h"

/// <summary>
/// UI を一つ表示するクラス
/// プレイヤーの居合に対応させているが抽象クラス化したい
/// </summary>
class UIPanel
{
public:
	struct UIData
	{
		lwp::Vector2 anchor_ = { 0.0f,0.0f };
		lwp::Vector2 transform_ = { 0.0f,0.0f };
		std::string fileName_ = "";
	};

public:

	UIPanel() = default;
	UIPanel(const UIData& data);
	virtual ~UIPanel() = default;

	// 初期化
	virtual void Initialize();
	virtual void Initialize(const std::string& fileName);
	virtual void Initialize(const UIData& data);

	// 更新
	virtual void Update();

public:	//*** ゲッターセッター ***//

	// アンカーポイント
	void SetAnchorPoint(const lwp::Vector2& a) { uiSprite_->anchorPoint = a; }
	// スクリーン座標系でピクセル単位の設定
	void SetPosition(const lwp::Vector2& pos) { world_.translation.x = pos.x; world_.translation.y = pos.y; }
	void SetPositionX(float x) { world_.translation.x = x; }
	void SetPositionY(float y) { world_.translation.y = y; }
	// サイズ設定
	void SetSize(int x, int y) { uiSprite_->size = lwp::Vector2(float(x), float(y)); }
	void SetSize(const lwp::Vector2& size) { uiSprite_->size = size; }
	// 描画するかどうかの設定
	void SetIsActive(bool flag) { uiSprite_->isActive = flag; }

protected:

	// ワールド座標
	// 2D での利用
	lwp::WorldTransform world_;

	// スプライト
	lwp::Sprite* uiSprite_ = nullptr;
};