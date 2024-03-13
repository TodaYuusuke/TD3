#pragma once
#include "../Adapter/Adapter.h"

class FollowCamera
{
public:
	/// 
	/// Default Method
	/// 

	// 初期化
	//void Initialize();

	// 更新処理
	void Update();

	/// 
	/// User Method
	/// 

	// ジャスト抜刀の瞬間カメラを縮小
	void ReduceFov();

	// ジャスト抜刀終了時のイージング
	void ResetFov();

	/// Getter
	
	/// Setter
	// 追従する対象を設定
	void SetTarget(const LWP::Object::WorldTransform* target) { target_ = target; }

	// メインカメラのアドレスを設定
	void SetCameraAddress(lwp::Camera* pCamera) { pCamera_ = pCamera; }

private:// プライベートな関数
	// カメラの角度を入力
	void InputAngle();


public:// パブリックな変数
	LWP::Object::Camera* pCamera_;

private:// メンバ変数

	// 追従対象
	const LWP::Object::WorldTransform* target_ = nullptr;
};