#pragma once
#include <scene/IScene.h>
#include "Game/Components/FollowCamera/FollowCamera.h"

class Sample final
	: public IScene {
public:
	Sample() = default;	// コンストラクタは自由に定義可能
	~Sample() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	LWP::Primitive::Triangle* tri[2];
	LWP::Primitive::Surface* surface;
	LWP::Primitive::Sphere* sphere;
	
	LWP::Primitive::Mesh* cubeModel;
	LWP::Primitive::Mesh* stressTestModel;	// 描画負荷検証用モデル

	LWP::Resource::Texture* uvTexture;
	LWP::Resource::Texture* monsterBall;

	// SEデータ
	LWP::Resource::Audio* audio;

	// サブカメラ
	LWP::Object::Camera* subCamera;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// パーティクル
	LWP::Object::Particle* particle;

	bool useMonsterBall = true;
};