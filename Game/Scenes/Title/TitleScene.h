#pragma once
#include <scene/IScene.h>
#include "../../SceneTransition/SceneTransition.h"

class TitleScene final
	: public IScene
{
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

private: //*** 変数群 ***//

	// 画面全体の画像
	LWP::Primitive::Sprite backSprite_;

	// シーン遷移
	std::unique_ptr<SceneTransition> sceneTransition_;
	//BGM
	std::unique_ptr<LWP::Resource::Audio> audio;
	int aaa = 0;
};