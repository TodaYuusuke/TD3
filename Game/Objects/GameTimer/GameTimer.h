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

	// 描画するかどうか
	bool isActive_ = false;
	// カウントが終わっているかフラグ
	bool isEnd_ = false;

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
	/// タイマーを指定した秒数でリセット
	/// </summary>
	/// <param name="limit">秒数</param>
	void Reset(uint32_t limit);

	/// <summary>
	/// 時間を進める時だけ進める
	/// </summary>
	void Update();

	//*** Getter ***//

	/// <summary>
	/// 経過秒数を取得
	/// </summary>
	uint32_t GetCurrentSecond() const { return currentSec_; }
	/// <summary>
	/// 耐久秒数を取得
	/// </summary>
	uint32_t GetLimitSecond() const { return limitSec_; }
	/// <summary>
	/// 耐久する時間と経過した時間で t を得る
	/// </summary>
	/// <returns>t : 0.0f ~ 1.0f</returns>
	float GetSecondT() const { return (float)currentSec_ / (float)limitSec_; }

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
	// 耐久する最大時間
	uint32_t limitSec_ = 0u;

	// タイマーを表示する場所
	lwp::Vector3 timerPosition_;

	// 1 秒
	GameCounter countS0_;
	// 10
	GameCounter countS1_;
	// 1 分
	GameCounter countM0_;
	// 10
	GameCounter countM1_;


private: //*** プライベート関数 ***//

	/// <summary>
	/// デバッグ
	/// </summary>
	void DebugWindow();





};