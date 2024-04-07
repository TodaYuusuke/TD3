#pragma once
#include <Adapter.h>

#include "Game/Objects/Experience/ExpManager.h"

struct MotionWork
{
	lwp::Vector3 targetpoint;//目標地点
	float t;//媒介変数
	float speed;//媒介変数の増えるスピード
	bool flag = false;//再生のフラグ
};

class Player;

class IEnemy
{
public:
	// デストラクタ
	virtual ~IEnemy() = default;

	void Initialize();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Attack() = 0;

	/// <summary>
	/// 死んだ瞬間に呼び出す関数
	/// </summary>
	void Death();

	/// <summary>
	/// 死亡時の処理
	/// </summary>
	virtual void Dying();
	/// <summary>
	/// 死亡時のアニメーション
	/// </summary>
	void DyingAnimation();

public: //*** ゲッターセッター ***//

	/// <summary>
	/// この実態が機能しなくなっているかのフラグ
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() const { return isActive_; }
	const lwp::Vector3& GetPosition() { return models_[0].transform.translation; }

	// 狙う対象をセット(今回は自機をセットする)
	virtual void SetTarget(Player* player) { player_ = player; }
	virtual void SetPosition(lwp::Vector3 pos) { models_[0].transform.translation = pos; }
	void SetManager(ExpManager* p) { manager_ = p; }

protected: //*** 継承クラスで呼び出す共通処理 ***//


	/// <summary>
	/// 当たり判定に対する諸々の処理
	/// </summary>
	virtual void CreateCollider();

	/// <summary>
	/// 衝突判定を確認する関数
	/// </summary>
	/// <param name="data">ヒットデータ</param>
	virtual void OnCollision(const lwp::Collider::HitData& data);

	/// <summary>
	/// 指定した分 HP を削る
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void DecreaseHP(int damage);

protected:
	std::vector<LWP::Primitive::Mesh> models_;

	// 敵の当たり判定
	lwp::Collider::AABB collider_;

	// プレイヤーのポインタ
	Player* player_;
	// マネージャーのポインタ
	ExpManager* manager_;

	// 攻撃のクールタイム
	int attackWaitTime_;
	// 攻撃開始フラグ
	bool isAttack = false;
	// 死んだときに立つフラグ(アニメーション用)
	bool IsDead_ = false;
	// 死んだ時用アニメ
	MotionWork deadAnime = {
		// 下に沈んでいく速度
		.speed = 0.5f,
	};
	int deadFlame = 0;
	// 生きているかどうか
	bool isActive_ = true;

	// 敵ごとのHP
	// 初期化の時に設定する
	// 今のフェーズ数とかを参照できれば可変にできる？
	// 初期値を 20 くらいにしといて、プレイヤーの攻撃力が上がっても対応させるようにしたい
	int hp_ = 1;


	// 攻撃に当たった時の無敵時間(活動できない)を発生させる
	// 攻撃に当たった時と別の判定に当たった時の処理は分ける

	// 何かしらの攻撃に当たった(当たったことを検知)
	bool wasHitAttack_ = false;

	// 無量空処かどうか
	bool isUtopia = false;

	// 硬直、内部の処理が進行しない時間
	// 当たった攻撃とか、場合や種類によって時間を設定する
	// この時間が 0 になるまで動けない
	float utopiaTime_ = 0.0f;

};