#pragma once
#include <Adapter.h>

class SceneTransition
{
public:
	enum class SceneTransitionState {
		None,
		FadeIn,
		FadeOut
	};
public:
	SceneTransition() = default;
	~SceneTransition() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// シーンが切り替わる前の演出
	/// </summary>
	void PreSceneChange();

	/// <summary>
	/// シーンが切り替わる後の演出
	/// </summary>
	void PostSceneChange();

	/// <summary>
	/// シーンが切り替わったかを確認
	/// </summary>
	void CheckSceneChange();

	/// <summary>
	/// シーン遷移開始
	/// </summary>
	void Start();

	/// <summary>
	/// 
	/// </summary>
	void Reset() { isSceneChange_ = false; }

	bool GetIsSceneChange() { return isSceneChange_; }
public:
	static int sceneTransitionState_;
	static int preState_;

	// シーン遷移開始の信号
	static bool isStart_;
public:
	// 使用するスプライト
	LWP::Primitive::Sprite preSceneTransition_[3];
	LWP::Primitive::Sprite postSceneTransition_[3];
	// アニメーション
	LWP::Resource::Motion preSceneChange_;
	LWP::Resource::Motion postSceneChange_;

	bool isSceneChange_;
};