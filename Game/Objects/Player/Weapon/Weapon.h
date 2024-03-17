#pragma once

#include "Adapter.h"

#include <optional>
#include <array>

class Weapon final
{
public:

	enum class Behavior : unsigned int
	{
		Root = 0u,	// 待機状態
		Slash,		// 居合
		Moment,		// 後隙

		_COUNT,		// カウント用
	};

private:

	struct Range
	{
		lwp::Vector3 start_;
		lwp::Vector3 end_;
	};

	// 共通データ
	struct BaseData
	{
		Range scale_;
		Range rotate_;
		Range translate_;
	};

	struct RootData : public BaseData
	{

	};
	struct SlashData : public BaseData
	{

	};
	struct MomentData : public BaseData
	{

	};

public: //*** パブリック関数 ***//

	// コンストラクタ
	Weapon() = default;
	// デストラクタ
	~Weapon() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();

public:	//*** セッター,ゲッター ***//

	lwp::WorldTransform* GetWorldTransform() { return &demoModel_->transform; }
	void SetParent(lwp::WorldTransform* p) { demoModel_->transform.Parent(p); }

	void SetIsActive(bool flag) { demoModel_->isActive = flag; }
	void SetTPointer(float* p) { pT_ = p; }

	void SetBehavior(Behavior b) { reqBehavior_ = b; }

	// 武器のモデル取得
	lwp::Mesh* GetMesh() { return demoModel_; }

private: //*** プライベート関数 ***//

	void InitRoot();
	void InitSlash();
	void InitMoment();

	void UpdateRoot();
	void UpdateSlash();
	void UpdateMoment();

	void InitDatas();
	RootData* InitRootData();
	SlashData* InitSlashData();
	MomentData* InitMomentData();

private: //*** プライベート変数 ***//

	//*** 外部からのデータ ***//

	// 状態のデータ
	std::unique_ptr<RootData> rootData_;
	std::unique_ptr<SlashData> slashData_;
	std::unique_ptr<MomentData> momentData_;


	//*** 計算用 ***//

	// モデル
	lwp::Mesh* demoModel_ = nullptr;

	// イージング用
	// 外部からの T を参照
	float* pT_ = nullptr;

	// 状態
	Behavior behavior_ = Behavior::Root;
	std::optional<Behavior> reqBehavior_ = std::nullopt;


};

