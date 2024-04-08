#pragma once
#include "../Adapter/Adapter.h"

class Player;
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
	void SetTarget(const LWP::Object::WorldTransform* target) {
		target_ = target;
		ResetAngle();
	}

	// メインカメラのアドレスを設定
	void SetCameraAddress(lwp::Camera* pCamera) { pCamera_ = pCamera; }

	// 自機のアドレスを設定
	void SetPlayer(Player* player) { player_ = player; }

	// 向きをリセットする
	void ResetAngle();

	// 抜刀のイージングをスタートする
	void StartSlash() {
		if (!isEffectEasing_) {
			fovState_ = FovState::REDUCE;
		}
	}
	// 抜刀時のイージングを終了する
	void EndSlash() {
		if (!isEffectEasing_) {
			fovState_ = FovState::RESET;
		}
	}

	// 抜刀以外のfovイージングを開始
	void StartEffectFov(const float& goalFov) {
		effectGoalFov_ = goalFov;
		fovState_ = FovState::EFFECT;
		isEffectEasing_ = true;
	}
	// 抜刀以外のfovイージングを終了
	void EndEffectFov() {
		fovState_ = FovState::RESET;
		endFrame_ = kJustSlashFrame;
		isEffectEasing_ = false;
	}

private:/// プライベートな関数
	// キーボード,ゲームパッドの入力処理
	void InputAngle();

	// 他の演出でFovの数値をイージングするときに使用する
	void StartFovEasing();

	///
	/// ジャスト抜刀時に呼ばれる処理
	/// 

	// ジャスト抜刀の瞬間カメラを縮小
	void ReduceFov(const float& goalFov);
	// ジャスト抜刀終了時のイージング
	void ResetFov();
	// ジャスト抜刀の更新処理
	void JustSlashUpdate();

	///
	/// 数学関数
	///

	// 返り値がfloatの線形補間
	float Lerp(const float& v1, const float& v2, float t);
	// 最短角度補間
	float LerpShortAngle(float a, float b, float t);

	// オフセットの計算
	LWP::Math::Vector3 CalcOffset() const;

private:/// 定数
	///
	/// 視点操作の設定
	/// 

	// x軸の最低値
	const float kMinAxisX = 0.06f;
	// x軸の最大値
	const float kMaxAxisX = 0.4f;
	// 入力感度
	const LWP::Math::Vector2 kSensitivity = { 0.05f, 0.02f };
	// 視点移動の滑らかさ(0~1の間で設定)
	const float kRotationSmoothness = 0.2f;
	// カメラの後追い速度(0~1の間で設定)
	const float kFollowRate = 0.25f;

	// 追従対象との距離
	const LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,-30.0f };

	// 初期角度 
	LWP::Math::Vector3 kStartAngle = { 0.2f, 0.0f, 0.0f };

	///
	/// ジャスト抜刀時のFov設定
	/// 

	// Fovの最大値
	const float kMaxFov = 90;

	// 通常抜刀のイージング終了フレーム
	const float kNormalSlashFrame = 30;
	// ジャスト抜刀のイージング終了フレーム
	const float kJustSlashFrame = 90;

public:/// パブリックな変数
	LWP::Object::Camera* pCamera_;

private:/// プライベートな変数
	// 自機のアドレス
	Player* player_;

	// 追従対象
	const LWP::Object::WorldTransform* target_ = nullptr;
	// 追従対象の残像座標
	LWP::Math::Vector3 interTarget_;
	// 目標角度
	LWP::Math::Vector2 destinationAngle_;

	// Fovの状態
	enum FovState {
		NORMAL,		// 何もない
		REDUCE,		// 縮小
		RESET,		// 元の数値に戻す
		EFFECT		// 抜刀以外のfovのイージング
	};
	// Fovの状態
	FovState fovState_;
	FovState preFovState_;
	// 前のFovの値を保存
	float tempFov_;
	// 縮小するFovの目標値
	float goalFov_;
	// 抜刀以外に使用するfovの目標値
	float effectGoalFov_;

	// 現在の時間(frame単位)
	float currentFrame_;
	// イージング終了時間
	float endFrame_;
	// イージングスタート
	bool isStartEase_;
	// イージング終了
	bool isEndEase_;

	// 後追いのレート
	float followRate_;

	// 抜刀以外のイージング
	bool isEffectEasing_;
};