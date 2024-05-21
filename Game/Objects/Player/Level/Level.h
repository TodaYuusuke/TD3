#pragma once

#include <Adapter.h>

#include "ExpBar.h"

class Player;

class Level
{
public: //*** パブリック関数 ***//

	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	Level() = default;
	~Level();

	// 初期化
	void Initialize(Player* p);

	// 更新
	void Update();

public: //*** ゲッターセッター ***//


private: //*** プライベート関数 ***//

	// 当たり判定生成
	void CreateCollision();

	// ヒット
	void OnCollision(const lwp::Collider::HitData& data);

	// 経験値取得
	void GainEXP();

	// レベルアップ
	void LevelUp();

#ifdef DEMO

	void DebugWindow();

#endif // DEMO

private: //*** プライベート変数 ***//

	// 前のフレームの位置
	lwp::Vector3 prePos_;
	// プレイヤーのポインタ
	Player* player_ = nullptr;

	std::unique_ptr<ExpBar> bar_;

	// 今のレベル
	uint32_t lv_ = 1u;

	// 次のレベルアップに必要な経験値量
	float reqEXP_;

	// 今蓄えている経験値
	float exp_ = 0.0f;

	// プレイヤーの経験値取得範囲
	//lwp::Collider::Capsule* collider_ = nullptr;
	lwp::Collider::Capsule collider_;

};

