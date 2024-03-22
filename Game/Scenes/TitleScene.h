#pragma once
#include <scene/IScene.h>

#pragma region GameInclude
#include "Game/Objects/FollowCamera/FollowCamera.h"
#include "Game/Objects/Player/Player.h"
#include "Game/Objects/Level/Level.h"
#include "Game/Objects/Enemy/EnemyManager.h"
#include "Game/Objects/Experience/ExpManager.h"
#pragma endregion

class TItleScene final
	: public IScene
{
public:
	TItleScene() = default;	// コンストラクタは自由に定義可能
	~TItleScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

public: //*** 関数群 ***//

	// ジャスト抜刀
	void StartJustSlash();

private: //*** 変数群 ***//

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<EnemyManager> enemyManager_;

	// 経験値マネージャー
	std::unique_ptr<ExpManager> expManager_;

	// プレイヤー
	std::unique_ptr<Player> player_;

	// レベルアップ機能
	std::unique_ptr<Level> level_;

	// ジャスト抜刀
	bool isJustSlash_ = false;

	// ジャストからの時間
	float time_ = 0.0f;

	// スローモーションになる時間
	float cTIMESLOW_ = 0.5f;
};