#include "FollowCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace LWP::Input::Pad;

FollowCamera::FollowCamera() {
	currentFrame_ = 0;
	isStartEase_ = false;
	isEndEase_ = false;
	fovState_ = FovState::NORMAL;
	preFovState_ = fovState_;
}

void FollowCamera::Update() {
	// 入力処理
	InputAngle();

	// 追従の計算
	if (target_) {
		Vector3 offset = { 0.0f, 0.0f, -30.0f };;
		// カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);

		// オフセットをカメラの回転に合わせて回転
		offset = offset * rotateMatrix;

		// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
		pCamera_->transform.translation = target_->translation + offset;
	}

	// ジャスト抜刀時のfovの更新処理
	JustSlashUpdate();
}

void FollowCamera::InputAngle() {
	// rotationが2πより大きかったら(もしくは-2πより小さ勝ったら)0にリセット
	// Y軸
	if (pCamera_->transform.rotation.y >= 6.28f || pCamera_->transform.rotation.y <= -6.28f) {
		pCamera_->transform.rotation.y = 0.0f;
	}
	// X軸
	if (pCamera_->transform.rotation.x >= 6.28f || pCamera_->transform.rotation.x <= -6.28f) {
		pCamera_->transform.rotation.x = 0.0f;
	}

	// 入力感度
	const Math::Vector2 sensitivity = { 0.05f, 0.02f };

#pragma  region キーボード入力
	// Y軸
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		pCamera_->transform.rotation.y -= sensitivity.x;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		pCamera_->transform.rotation.y += sensitivity.x;
	}
	// X軸
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		pCamera_->transform.rotation.x += sensitivity.y;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		pCamera_->transform.rotation.x -= sensitivity.y;
	}
#pragma endregion

#pragma  region ゲームパッド入力
	// Y軸
	pCamera_->transform.rotation.y += Controller::GetRStick().x * sensitivity.x;
	// X軸
	pCamera_->transform.rotation.x -= Controller::GetRStick().y * sensitivity.y;
#pragma endregion
}

void FollowCamera::ReduceFov() {
	const float kFinishFrame = 90;
	float t = Utility::Easing::OutExpo(currentFrame_ / kFinishFrame);

	if (t < 1.0f) {
		pCamera_->fov = Lerp(tempFov_, kMinFov, t);
		currentFrame_++;
	}
	if (t >= 1.0f) {
		fovState_ = FovState::NORMAL;
	}
}

void FollowCamera::ResetFov() {
	const float kFinishFrame = 10;
	float t = Utility::Easing::OutExpo(currentFrame_ / kFinishFrame);
	if (t <= 1.0f) {
		pCamera_->fov = Lerp(tempFov_, kMaxFov, t);
		currentFrame_++;
	}
	if (t >= 1.0f) {
		fovState_ = FovState::NORMAL;
	}
}

float FollowCamera::Lerp(const float& v1, const float& v2, float t) {
	return v1 + ((v2 - v1) * t);
}

void FollowCamera::StartJustSlash() {
	fovState_ = FovState::REDUCE;
}

void FollowCamera::EndJustSlash() {
	fovState_ = FovState::RESET;
}

void FollowCamera::JustSlashUpdate() {
	// イージング開始前と開始後にfovの値を1フレームだけ保存
	if (fovState_ != preFovState_) {
		currentFrame_ = 0;
		tempFov_ = pCamera_->fov;
	}
	// 前のフレームのfov状態を代入
	preFovState_ = fovState_;

	switch (fovState_) {
	case FollowCamera::NORMAL:		
		break;
	case FollowCamera::REDUCE:
		ReduceFov();
		break;
	case FollowCamera::RESET:
		ResetFov();
		break;
	}
}