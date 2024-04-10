#pragma once

#include <Adapter.h>

/// <summary>
/// プレイヤーの HP を表示する
/// </summary>
class PlayerHP
{
public: //*** パブリック関数 ***//

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
	void Decrease();
	/// <summary>
	/// 最大 HP を増やす
	/// </summary>
	void IncreaseMax();
	/// <summary>
	/// 最大 HP を減らす
	/// </summary>
	void DecreaseMax();


private: //*** プライベート変数 ***//

	lwp::Sprite sprite_;

	LWP::Utility::Observer<int> hp_ = 0;

};

