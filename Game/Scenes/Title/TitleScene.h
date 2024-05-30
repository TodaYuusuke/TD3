#pragma once
#include <scene/IScene.h>
#include "../../SceneTransition/SceneTransition.h"

class TitleScene final : public IScene {
public:
	TitleScene() = default;	// コンストラクタは自由に定義可能
	~TitleScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

public: //*** 関数群 ***//

public://*** 構造体と列挙型 ***//
	// 使用する画像の種類
	enum UseSprite {
		BackGround,		// 背景
		Text_TitleName,	// "斬り切り舞"
		Text_GamePad_A,	// "Aボタンでスタート"
		NumSprite		// 登録したスプライト数
	};
private: //*** 変数群 ***//

	// タイトル画面に使用する画像
	std::vector<LWP::Primitive::Sprite> backSprite_;

	// シーン遷移
	std::unique_ptr<SceneTransition> sceneTransition_;

	// 
	float titleNameAnimFrame_;
};