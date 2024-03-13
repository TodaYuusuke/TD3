#include "FollowCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Input;
using namespace LWP::Input::Pad;

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	// 追従対象からカメラまでのオフセット
	offset_ = { 0.0f, 0.0f, -30.0f };
	viewProjection_.transform.rotation.x = 0.2f;
}

void FollowCamera::Update() {
	// 入力処理
	InputAngle();

	// 追従の計算
	if (target_) {
		Vector3 offset = offset_;
		// カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = LWP::Math::Matrix4x4::CreateRotateXYZMatrix(viewProjection_.transform.rotation);

		// オフセットをカメラの回転に合わせて回転
		offset = TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
		viewProjection_.transform.translation = target_->translation + offset;
	}

#ifdef _DEBUG
	ImGui::Begin("followCamera");
	ImGui::Text("Key Info  Arrow:Angle");
	if (ImGui::TreeNode("WorldTransform")) {
		ImGui::DragFloat3("translation", &viewProjection_.transform.translation.x, 0.01f, -100, 100);
		ImGui::DragFloat3("rotation", &viewProjection_.transform.rotation.x, 0.001f, -6.28f, 6.28f);
		ImGui::TreePop();
	}
	ImGui::DragFloat3("offset", &offset_.x, 0.01f, -100, 100);
	ImGui::End();
#endif
}

void FollowCamera::InputAngle() {
	// rotationが2πより大きかったら(もしくは-2πより小さ勝ったら)0にリセット
	// Y軸
	if (viewProjection_.transform.rotation.y >= 6.28f || viewProjection_.transform.rotation.y <= -6.28f) {
		viewProjection_.transform.rotation.y = 0.0f;
	}
	// X軸
	if (viewProjection_.transform.rotation.x >= 6.28f || viewProjection_.transform.rotation.x <= -6.28f) {
		viewProjection_.transform.rotation.x = 0.0f;
	}

	// 入力感度
	const Math::Vector2 sensitivity = { 0.05f, 0.02f };

#pragma  region キーボード入力
	// Y軸
	if (LWP::Input::Keyboard::GetPress(DIK_RIGHT)) {
		viewProjection_.transform.rotation.y -= sensitivity.x;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_LEFT)) {
		viewProjection_.transform.rotation.y += sensitivity.x;
	}
	// X軸
	if (LWP::Input::Keyboard::GetPress(DIK_UP)) {
		viewProjection_.transform.rotation.x += sensitivity.y;
	}
	if (LWP::Input::Keyboard::GetPress(DIK_DOWN)) {
		viewProjection_.transform.rotation.x -= sensitivity.y;
	}
#pragma endregion

#pragma  region ゲームパッド入力
	// Y軸
	viewProjection_.transform.rotation.y += Controller::GetRStick().x * sensitivity.x;
	// X軸
	viewProjection_.transform.rotation.x -= Controller::GetRStick().y * sensitivity.y;
#pragma endregion
}

LWP::Math::Vector3 FollowCamera::TransformNormal(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2];
	return result;
}