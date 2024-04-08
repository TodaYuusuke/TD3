#include "FollowCamera.h"
#include "../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace LWP::Input::Pad;

FollowCamera::FollowCamera() {
	currentFrame_ = 0;
	endFrame_ = 0;
	isStartEase_ = false;
	isEndEase_ = false;
	fovState_ = FovState::NORMAL;
	preFovState_ = fovState_;
	followRate_ = kFollowRate;
}

void FollowCamera::Update() {
	// 入力処理
	InputAngle();

	// 追従の計算
	if (target_) {
		// 追従座標の補間
		interTarget_ = LWP::Math::Vector3::Slerp(interTarget_, target_->translation, followRate_);

		// オフセットの計算
		LWP::Math::Vector3 offset = CalcOffset();
		// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
		pCamera_->transform.translation = interTarget_ + offset;
	}

	// ジャスト抜刀時のFovの更新処理
	JustSlashUpdate();

	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("main:rotate", &pCamera_->transform.rotation.x, 0, -6.28f, 6.28f);
	ImGui::End();

}

void FollowCamera::ResetAngle() {
	destinationAngle_ = { kStartAngle.x, target_->rotation.y };
	// オフセットの計算
	LWP::Math::Vector3 offset = CalcOffset();
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

	// 上下の回転に制限をかける
	destinationAngle_.x = std::clamp<float>(destinationAngle_.x, kMinAxisX, kMaxAxisX);

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

void FollowCamera::StartFovEasing() {
	float t = Utility::Easing::OutExpo(currentFrame_ / 90);

	if (t < 1.0f) {
		pCamera_->fov = Lerp(tempFov_, effectGoalFov_, t);
		currentFrame_++;
	}
	if (t >= 1.0f) {
		fovState_ = FovState::NORMAL;
	}
}

#pragma region ジャスト抜刀
void FollowCamera::ReduceFov(const float& goalFov) {
	float t = Utility::Easing::OutExpo(currentFrame_ / endFrame_);

	if (t < 1.0f) {
		pCamera_->fov = Lerp(tempFov_, goalFov, t);
		currentFrame_++;
	}
	if (t >= 1.0f) {
		fovState_ = FovState::NORMAL;
	}
}

void FollowCamera::ResetFov() {
	float t = Utility::Easing::OutExpo(currentFrame_ / endFrame_);

	if (t <= 1.0f) {
		pCamera_->fov = Lerp(tempFov_, kMaxFov, t);
		currentFrame_++;
	}
	if (t >= 1.0f) {
		fovState_ = FovState::NORMAL;
	}
}

void FollowCamera::JustSlashUpdate() {
	// ジャスト抜刀の場合
	if (player_->GetIsJustSlashing() && player_->GetIsSlash()) {
		goalFov_ = 65;
		// イージング終了時間
		endFrame_ = kJustSlashFrame;
	}
	// 通常抜刀の場合
	else if (!player_->GetIsJustSlashing() && player_->GetIsSlash()) {
		goalFov_ = 85;
		// イージング終了時間
		endFrame_ = kNormalSlashFrame;
	}

	// イージング開始前と開始後にfovの値を1フレームだけ保存
	if (fovState_ != preFovState_) {
		currentFrame_ = 0;
		tempFov_ = pCamera_->fov;
	}
	// 前のフレームのfov状態を代入
	preFovState_ = fovState_;

	switch (fovState_) {
	case FollowCamera::NORMAL:
		//followRate_ = 1.0f;
		break;
	case FollowCamera::REDUCE:
		// カメラを後追いさせる
		//followRate_ = kFollowRate;
		// 視野角を小さくする
		ReduceFov(goalFov_);
		break;
	case FollowCamera::RESET:
		// 視野角を大きくする
		ResetFov();
		break;
	case FollowCamera::EFFECT:
		// 
		StartFovEasing();
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

LWP::Math::Vector3 FollowCamera::CalcOffset() const {
	LWP::Math::Vector3 offset = kTargetDist;
	// カメラの角度から回転行列を計算
	LWP::Math::Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pCamera_->transform.rotation);
	// オフセットをカメラの回転に合わせて回転
	offset = offset * rotateMatrix;

	return offset;
}
#pragma endregion