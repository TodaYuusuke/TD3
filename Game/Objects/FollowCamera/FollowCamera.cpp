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
		// 追従座標の補間
		interTarget_ = LWP::Math::Vector3::Lerp(interTarget_, target_->translation, kFollowRate);
		
		// オフセットの計算
		Vector3 offset = CalcOffset();
		// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
		pCamera_->transform.translation = interTarget_ + offset;
	}

	// ジャスト抜刀時のFovの更新処理
	JustSlashUpdate();
}

void FollowCamera::ResetAngle() {
	destinationAngle_ = { kStartAngle.x, target_->rotation.y };
	// オフセットの計算
	Vector3 offset = CalcOffset();
	// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
	pCamera_->transform.translation = interTarget_ + offset;
}

void FollowCamera::InputAngle() {
	// 入力感度
	const LWP::Math::Vector2 sensitivity = kSensitivity;
#pragma  region キーボード入力
	// Y軸
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		destinationAngle_.y += sensitivity.x;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		destinationAngle_.y -= sensitivity.x;
	}
	// X軸
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		destinationAngle_.x -= sensitivity.y;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		destinationAngle_.x += sensitivity.y;
	}
#pragma endregion

#pragma  region ゲームパッド入力
	// Rスティック
	destinationAngle_.x -= Controller::GetRStick().y * sensitivity.y;
	destinationAngle_.y += Controller::GetRStick().x * sensitivity.x;

	// Rスティック押し込み
	// 角度リセット
	if (Controller::GetTrigger(XINPUT_GAMEPAD_RIGHT_THUMB)) {
		ResetAngle();
	}
#pragma endregion

	// 最短角度補間
	pCamera_->transform.rotation.y = LerpShortAngle(pCamera_->transform.rotation.y, destinationAngle_.y, kRotationSmoothness);
	pCamera_->transform.rotation.x = LerpShortAngle(pCamera_->transform.rotation.x, destinationAngle_.x, kRotationSmoothness);
}

#pragma region ジャスト抜刀
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
#pragma endregion

#pragma region 数学関数
float FollowCamera::Lerp(const float& v1, const float& v2, float t) {
	float result = v1 + (v2 - v1) * t;
	return result;
}

float FollowCamera::LerpShortAngle(float a, float b, float t) {
	// 角度差分を求める
	float diff = b - a;

	float pi = 3.14f;

	diff = std::fmod(diff, 2 * pi);
	if (diff < 2 * -pi) {
		diff += 2 * pi;
	}
	else if (diff >= 2 * pi) {
		diff -= 2 * pi;
	}

	diff = std::fmod(diff, 2 * pi);
	if (diff < -pi) {
		diff += 2 * pi;
	}
	else if (diff >= pi) {
		diff -= 2 * pi;
	}

	return a + diff * t;
}

LWP::Math::Vector3 FollowCamera::CalcOffset() {
	Vector3 offset = kTargetDist;
	// カメラの角度から回転行列を計算
	Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
	// オフセットをカメラの回転に合わせて回転
	offset = offset * rotateMatrix;

	return offset;
}
#pragma endregion