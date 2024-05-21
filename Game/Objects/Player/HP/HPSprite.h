#pragma once
#include <Adapter.h>


class HPSprite
{
public:

	// 後ろ
	lwp::Sprite subSprite;

	// ハート
	lwp::Sprite mainSprite;

	// 後ろの画像の倍率
	float subScale = 0.55f;

	// 元の位置
	lwp::Vector3 basePosition{ 20.0f,20.0f,0.0f };

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetPosition(const lwp::Vector3& pos)
	{
		basePosition = pos;
	}

	std::string GetTexturePass()
	{
		return "white.png";
	}
};

