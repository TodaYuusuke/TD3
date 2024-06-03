#include "SceneTransition.h"

using namespace LWP;
using namespace LWP::Utility;

void SceneTransition::Initialize() {
	// UI
	for (int i = 0; i < 2; i++) {
		// シーン切り替え前の演出
		preSceneTransition_[i].texture = lwp::LoadTexture("white.png");
		preSceneTransition_[i].isActive = false;
		preSceneTransition_[i].name = "SceneTransition";
		preSceneTransition_[i].isUI = true;
		preSceneTransition_[i].transform.translation.x = 1980.0f;
		preSceneTransition_[i].transform.scale = { 9.0f,5,0 };

		// シーン切り替え後の演出
		postSceneTransition_[i].texture = lwp::LoadTexture("white.png");
		postSceneTransition_[i].isActive = false;
		postSceneTransition_[i].name = "SceneTransition";
		postSceneTransition_[i].isUI = true;
		postSceneTransition_[i].transform.translation.x = 0.0f;
		postSceneTransition_[i].transform.scale = { 7.5f,5,0 };
	}
	// シーン切り替え前の演出
	preSceneTransition_[2].texture = lwp::LoadTexture("Transition.png");
	preSceneTransition_[2].isActive = false;
	preSceneTransition_[2].name = "SceneTransition";
	preSceneTransition_[2].isUI = true;
	preSceneTransition_[2].transform.translation.x = 1980.0f;

	// シーン切り替え後の演出
	postSceneTransition_[2].texture = lwp::LoadTexture("Transition.png");
	postSceneTransition_[2].isActive = false;
	postSceneTransition_[2].name = "SceneTransition";
	postSceneTransition_[2].isUI = true;
	postSceneTransition_[2].transform.translation.x = 0.0f;

#pragma region 色の設定
	// シーン遷移前の
	//preSceneTransition_[2].commonColor = new Utility::Color(Utility::ColorPattern::GREEN);
	preSceneTransition_[1].commonColor = new Utility::Color(Utility::ColorPattern::CYAN);
	preSceneTransition_[0].commonColor = new Utility::Color(Utility::ColorPattern::BLUE);

	//postSceneTransition_[2].commonColor = new Utility::Color(Utility::ColorPattern::GREEN);
	postSceneTransition_[1].commonColor = new Utility::Color(Utility::ColorPattern::CYAN);
	postSceneTransition_[0].commonColor = new Utility::Color(Utility::ColorPattern::BLUE);
#pragma endregion

#pragma region シーン遷移演出
	// シーン遷移前のアニメーション
	preSceneChange_.Add(&preSceneTransition_[0].transform.translation, lwp::Vector3{ -1980,0,0 }, 0, 0.5f, LWP::Utility::Easing::Type::OutExpo)
		.Add(&preSceneTransition_[1].transform.translation, lwp::Vector3{ -1980,0,0 }, 0.05f, 0.5f, LWP::Utility::Easing::Type::OutExpo)
		.Add(&preSceneTransition_[2].transform.translation, lwp::Vector3{ -1980,0,0 }, 0.1f, 0.5f, LWP::Utility::Easing::Type::OutExpo);
	// シーン遷移後のアニメーション
	postSceneChange_.Add(&postSceneTransition_[2].transform.translation, lwp::Vector3{ -2000,0,0 }, 0, 0.5f, LWP::Utility::Easing::Type::OutExpo)
		.Add(&postSceneTransition_[1].transform.translation, lwp::Vector3{ -2000,0,0 }, 0.05f, 0.5f, LWP::Utility::Easing::Type::OutExpo)
		.Add(&postSceneTransition_[0].transform.translation, lwp::Vector3{ -2000,0,0 }, 0.1f, 0.5f, LWP::Utility::Easing::Type::OutExpo);
#pragma endregion

	if (isDelayStart) {
		for (int i = 0; i < 3; i++) {
			postSceneTransition_[i].isActive = true;
			postSceneTransition_[i].transform.translation.x = 0.0f;
		}
	}
}

void SceneTransition::Update() {
	if (isStart_) {
		CheckSceneChange();

		if (isDelayStart) {
			delayFrame_--;
		}
	}
}

void SceneTransition::PreSceneChange() {
	// ボタンを押したらシーン遷移アニメーションを開始
	if (preState_ != sceneTransitionState_) {
		for (int i = 0; i < 3; i++) {
			preSceneTransition_[i].isActive = true;
		}
		preState_ = sceneTransitionState_;
		preSceneChange_.Start();
	}
	// シーン遷移前のアニメーション終了したらシーンが切り替わる
	if (preSceneChange_.isEnd()) {
		sceneTransitionState_ = (int)SceneTransitionState::FadeOut;
		isSceneChange_ = true;
		isDelayStart = true;
	}
}

void SceneTransition::PostSceneChange() {
	if (delayFrame_ <= 0 && isDelayStart) {
		for (int i = 0; i < 3; i++) {
			postSceneTransition_[i].isActive = true;
		}
		isDelayStart = false;
		preState_ = sceneTransitionState_;
		postSceneChange_.Start();
	}
	// アニメーション終了
	if (postSceneChange_.isEnd() && !isDelayStart) {
		sceneTransitionState_ = (int)SceneTransitionState::None;
		for (int i = 0; i < 3; i++) {
			postSceneTransition_[i].isActive = false;
			postSceneTransition_[i].transform.translation.x = -2000;
		}
		isStart_ = false;
	}
}

void SceneTransition::CheckSceneChange() {
	if (sceneTransitionState_ == (int)SceneTransitionState::None) {
		sceneTransitionState_ = (int)SceneTransitionState::FadeIn;
	}
	if (sceneTransitionState_ == (int)SceneTransitionState::FadeOut) {
		PostSceneChange();
	}
	if (sceneTransitionState_ == (int)SceneTransitionState::FadeIn) {
		PreSceneChange();
	}
}

int SceneTransition::sceneTransitionState_ = (int)SceneTransitionState::None;
int SceneTransition::preState_ = (int)SceneTransitionState::None;
bool SceneTransition::isStart_ = false;
int SceneTransition::delayFrame_ = 5;
bool SceneTransition::isDelayStart;