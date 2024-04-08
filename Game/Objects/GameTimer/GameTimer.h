#pragma once

#include <Adapter.h>

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

	/// <summary>
	/// デバッグ
	/// </summary>
	void DebugWindow();

private: //*** プライベート変数 ***//

	// 計測するかのフラグ
	bool isWatch_ = false;

	// 一秒経過するまでを計測する
	float checkSec_ = 0.0f;

	// 今の経過時間を格納
	uint32_t currentSec_ = 0u;

};