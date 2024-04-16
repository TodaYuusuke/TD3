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
	float subScale = 1.05f;

	// 元の位置
	lwp::Vector3 basePosition{ 20.0f,20.0f,0.0f };

public:

	void Initialize();


	void SetPosition(const lwp::Vector3& pos)
	{
		mainSprite.transform.translation = pos;
		subSprite.transform.translation = pos;
	}


	std::string GetTexturePass()
	{
		return "white.png";
	}
};

