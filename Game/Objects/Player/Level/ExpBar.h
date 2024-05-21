#pragma once
#include <Adapter.h>

class ExpBar
{
public:

	// 後ろ
	lwp::Sprite subSprite1;

	lwp::Sprite subSprite2;

	// バーの画像
	lwp::Sprite mainSprite;

	// 後ろの画像の倍率
	float subScale = 1.05f;

	// 元の画像の大きさ
	lwp::Vector2 preSize_{};

	// 元の位置
	// 画面下からの位置
	lwp::Vector3 basePosition{ 0.0f,100.0f,0.0f };

	// バーの大きさ
	lwp::Vector2 kBarSize_ = { 1000.0f,50.0f };

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(uint32_t exp, uint32_t num);

	void SetPosition(const lwp::Vector3& pos)
	{
		basePosition = pos;
	}

	std::string GetTexturePass()
	{
		return "white.png";
	}
};


