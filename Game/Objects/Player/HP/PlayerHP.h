#pragma once

#include <Adapter.h>

/// <summary>
/// プレイヤーの HP を表示する
/// </summary>
class PlayerHP
{
public: //*** パブリック関数 ***//

	// 今の HP
	int hp_ = 0;

	// 最大 HP
	int maxHp_ = 0;


	// wtf
	lwp::WorldTransform transform_;

	// 表示するか
	bool isActive_;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// HP を増やす
	/// </summary>
	void Increase();
	/// <summary>
	/// HP を減らす
	/// </summary>
	bool Decrease();
	/// <summary>
	/// 最大 HP を増やす
	/// </summary>
	void IncreaseMax();
	/// <summary>
	/// 最大 HP を減らす
	/// </summary>
	void DecreaseMax();


private: //*** プライベート変数 ***//

	// HP バーを固定のスケール指定できるようにする
	lwp::Vector3 kHPSpriteConvertScale_ = { 1.0f,1.0f,1.0f };

	// HP バーの基本サイズ
	lwp::Vector3 kHPBaseSize_ = { 500.0f,200.0f,1.0f };

	// HP の画像
	lwp::Sprite sprHp_;

private: //*** プライベート関数 ***//



};

