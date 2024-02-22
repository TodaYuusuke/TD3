#pragma once
#include "../Adapter/Adapter.h"

class FollowCamera
{
public:
	/// 
	/// Default Method
	/// 

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	/// 
	/// User Method
	/// 

	/// Getter
	/// ビュープロジェクションを取得
	const LWP::Math::Matrix4x4& GetViewProjection() { return viewProjection_.GetViewProjection(); }
	
	/// Setter
	// 追従する対象を設定
	void SetTarget(const LWP::Object::WorldTransform* target) { target_ = target; }
	// 追従対象とカメラの距離の設定
	void SetCameraToTargetDist(LWP::Math::Vector3 offset) { offset_ = offset; }

private:// プライベートな関数
	// カメラの角度を入力
	void InputAngle();

	LWP::Math::Vector3 TransformNormal(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix);

public:// パブリックな変数
	LWP::Object::Camera viewProjection_;

private:// メンバ変数
	// 追従対象
	const LWP::Object::WorldTransform* target_ = nullptr;
	// 追従対象からカメラまでのオフセット
	LWP::Math::Vector3 offset_;
};