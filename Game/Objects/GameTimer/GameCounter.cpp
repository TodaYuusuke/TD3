#include "GameCounter.h"

using namespace LWP;
using namespace LWP::Primitive;
using namespace LWP::Resource;


void GameCounter::Initialize(const std::string& textureName)
{
	sprite_ = new Sprite;

	textureName_ = textureName;
	// 画像を設定
	sprite_->texture = LoadTexture(GetTextruePass());
	sprite_->anchorPoint = { 0.5f,0.5f };
	sprite_->isUI = true;
	sprite_->isActive = false;
	sprite_->name = "Counter";
	transform_.translation = { 0.0f,0.0f };
	isActive_ = false;
	Reset();
}

void GameCounter::Update()
{
	sprite_->transform = transform_;
	sprite_->isActive = isActive_;
	if (count_ != preCount_)
	{
		ChangeTexture();
	}
}

void GameCounter::Reset(int cnt)
{
	count_ = cnt;
	preCount_ = count_;
	ChangeTexture();
}

bool GameCounter::Increase()
{
	// 増分
	count_++;
	// 値を保存
	preCount_ = count_;
	// 桁上がりしてたら
	if (10 <= count_)
	{
		// 0 に戻す
		count_ = 0;
		// 値を保存
		preCount_ = count_;
		ChangeTexture();
		return true;
	}
	ChangeTexture();
	return false;
}

bool GameCounter::Decrease()
{
	// 減分
	count_--;
	// 値を保存
	preCount_ = count_;
	// 桁下がりしてたら
	if (count_ < 0)
	{
		// 9 に戻す
		count_ = 9;
		// 値を保存
		preCount_ = count_;
		ChangeTexture();
		return true;
	}
	ChangeTexture();
	return false;
}

void GameCounter::ChangeTexture(const std::string& textureName)
{
	textureName_ = textureName;
	ChangeTexture();
}


std::string GameCounter::GetTextruePass()
{
	return "count/" + textureName_ + "/" + textureName_ + std::to_string(count_) + ".png";
}

void GameCounter::ChangeTexture()
{
	sprite_->texture = LoadTexture(GetTextruePass());
	sprite_->Update();
}

