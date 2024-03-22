#pragma once

#include <Adapter.h>

class Level
{
public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	Level() = default;
	~Level();

	// 初期化
	void Initialize();

	// 更新
	void Update();

private: //*** プライベート関数 ***//

#ifdef DEMO

	void DebugWindow();

#endif // DEMO


private: //*** プライベート変数 ***//

	// 次のレベルアップに必要な経験値量
	float reqEXP_ = 10.0f;

	// 今蓄えている経験値
	float exp_ = 0.0f;


};

