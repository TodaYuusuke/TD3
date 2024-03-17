#include "FollowCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace LWP::Input::Pad;

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
}


static int hhhh = 90;

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

#ifdef DEMO

	ImGui::Begin("TTT");
	ImGui::DragInt("value", &hhhh);
	ImGui::End();
		
	if (LWP::Input::Keyboard::GetPress(DIK_1)) {
		ReduceFov();
	}
	if (LWP::Input::Keyboard::GetPress(DIK_2)) {
		ResetFov();
	}

#endif
}

void FollowCamera::ReduceFov() {
	pCamera_->fov = hhhh;
}

void FollowCamera::ResetFov() {
	pCamera_->fov = 90;
}