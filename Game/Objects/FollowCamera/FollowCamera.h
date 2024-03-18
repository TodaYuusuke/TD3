#pragma once
#include "../Adapter/Adapter.h"

class FollowCamera
{
public:
	/// 
	/// Default Method
	/// 

	// コンストラクタ
	FollowCamera();

	// 初期化
	//void Initialize();

	// 更新処理
	void Update();

	/// 
	/// User Method
	/// 

	/// Getter

	/// Setter
	// 追従する対象を設定
	void SetTarget(const LWP::Object::WorldTransform* target) { target_ = target; }

	// メインカメラのアドレスを設定
	void SetCameraAddress(lwp::Camera* pCamera) { pCamera_ = pCamera; }

	// ジャスト抜刀時のイージングをスタートする
	void StartJustSlash();
	// ジャスト抜刀時のイージングを終了する
	void EndJustSlash();

private:// プライベートな関数
	// ジャスト抜刀の瞬間カメラを縮小
	void ReduceFov();

	// ジャスト抜刀終了時のイージング
	void ResetFov();

	// ジャスト抜刀の更新処理
	void JustSlashUpdate();

	// カメラの角度を入力
	void InputAngle();

	// 返り値がfloatの線形補間
	float Lerp(const float& v1, const float& v2, float t);

private:// 定数
	// Fovの最小値
	const float kMinFov = 65;
	// Fovの最大値
	const float kMaxFov = 90;

public:// パブリックな変数
	LWP::Object::Camera* pCamera_;

private:// メンバ変数

	// Fovの状態
	enum FovState {
		NORMAL, // 何もない
		REDUCE, // 縮小
		RESET   // 元の数値に戻す
	};
	// Fovの状態
	FovState fovState_;
	FovState preFovState_;

	// 追従対象
	const LWP::Object::WorldTransform* target_ = nullptr;

	// イージングスタート
	bool isStartEase_;
	// イージング終了
	bool isEndEase_;
	// Fovの値を一瞬だけ保存する
	bool isTempFovTrigger_;

	// 現在の時間(frame単位)
	float currentFrame_;
	// 現在のFovを保存
	float tempFov_;
};