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

	float Lerp(const float& v1, const float& v2, float t) {
		float result = v1 + (v2 - v1) * t;
		return result;
	}
	//BGM
	bool IsSceneChangeBegin = false;
	bool IsSceneChangeEnd = true;
	std::unique_ptr<LWP::Resource::Audio> BGM;
	float BGMvolume = 0.2f;
	float BGMt = 0.0f;

	std::unique_ptr<LWP::Resource::Audio> chooseSE;

	//読み込みが終わったかどうか
	bool IsloadEnd = true;
};

