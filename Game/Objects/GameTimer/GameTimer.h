#pragma once

#include <Adapter.h>

#include "GameCounter.h"


/// <summary>
/// シングルトンインスタンス
/// </summary>
class GameTimer final
{
public: //*** パブリック関数 ***//

	static GameTimer* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 計測開始
	/// <para>中断からも再開する</para>
	/// </summary>
	void Start();
	/// <summary>
	/// 一時中断
	/// </summary>
	void Stop();
	/// <summary>
	/// タイマーをリセットする
	/// </summary>
	void Reset();

	/// <summary>
	/// 時間を進める時だけ進める
	/// </summary>
	void Update();

private: //*** プライベート変数 ***//

	// 中心からの距離
	float kPaddingCenter_ = 50.0f;
	// 数字同士の距離
	float kPaddingNumber_ = 60.0f;

	// 計測するかのフラグ
	bool isWatch_ = false;

	// 一秒経過するまでを計測する
	float checkSec_ = 0.0f;

	// 今の経過時間を格納
	uint32_t currentSec_ = 0u;

	// タイマーを表示する場所
	lwp::Vector3 timerPosition_;

	// 秒
	GameCounter countS0_;
	GameCounter countS1_;
	// 分
	GameCounter countM0_;
	GameCounter countM1_;

private: //*** プライベート関数 ***//

	/// <summary>
	/// デバッグ
	/// </summary>
	void DebugWindow();


	


};