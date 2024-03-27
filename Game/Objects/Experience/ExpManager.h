#pragma once

#include <list>
#include <Adapter.h>

#include "Experience.h"

// 経験値を管理する
class ExpManager final
{
public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	ExpManager() = default;
	~ExpManager();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 生成して登録
	void Create(const lwp::Vector3& pos);

private: //*** プライベート関数 ***//

#ifdef DEMO

	void DebugWindow();

#endif // DEMO


private: //*** プライベート変数 ***//

	std::list<Experience*> exps_;

};

