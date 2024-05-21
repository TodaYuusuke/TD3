#pragma once
#include <Adapter.h>
#include "../../FollowCamera/FollowCamera.h"

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
protected:
	// 体のパーツ
	enum BodyParts {
		BODY,
		L_ARM,
		R_ARM,
		BODYPARTSCOUNT
	};

public:
	// デストラクタ
	virtual ~IEnemy() = default;

	void Initialize();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Attack() = 0;

	// ノックバックの更新処理
	void KnockBackUpdate();

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
	/// <summary>
	/// 指定した分 HP を削る
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void DecreaseHP(int damage);

	/// <summary>
	/// デバッグ表示
	/// 外部呼出し
	/// </summary>
	virtual void DebugPrint();


	/// <summary>
	/// 静的変数の初期化
	/// </summary>
	static void InitStaticVariable();

	/// <summary>
	/// 出現時のエフェクトが終了しているかをチェックしdeleteする
	/// </summary>
	void CheckSpawnEffect();


public: //*** ゲッターセッター ***//

	/// <summary>
	/// この実態が機能しなくなっているかのフラグ
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() const { return isActive_; }
	bool GetIsDeath() const { return IsDead_; }
	const lwp::Vector3& GetPosition() { return models_[0].transform.translation; }

	lwp::Vector3 GetPosition()const { return models_[0].transform.translation; }

	// 狙う対象をセット(今回は自機をセットする)
	virtual void SetTarget(Player* player) { player_ = player; }
	virtual void SetPosition(lwp::Vector3 pos) { models_[0].transform.translation = pos; }
	void SetHP(int HP) { hp_ = HP; }
	// カメラのアドレスを設定
	virtual void SetCamera(FollowCamera* camera) { followCamera_ = camera; }
	void SetManager(ExpManager* p) { manager_ = p; }
	void SetSpawnEffect(lwp::Vector3 pos) {
		// 出現時にパーティクルを出す
#ifdef DEMO
		spawnEffect_(8, models_[0].transform.translation);
#else
		spawnEffect_(kNumSpawnParticle, models_[0].transform.translation);
#endif // !DEMO


		lightPillar_.transform.translation = pos;
		lightPillarMotion_.Start();
	}


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

	virtual bool CheckSlash(uint32_t hitBelong);


	/// <summary>
	/// 時間を指定して無敵になる関数
	/// </summary>
	/// <param name="time">何秒無敵になるか</param>
	void BecomeInvincible(float time);
	/// <summary>
	/// 時間を指定して無量空処になる関数
	/// </summary>
	/// <param name="time">何秒無量空処になるか</param>
	void BecomeUtopia(float time);


	// 無敵状態や無量空処状態を計算する
	void CheckFlags();

protected:// 定数
	// ボスの大きさ
	const LWP::Math::Vector3 kBossSize = { 4,5,4 };

	// ノックバックが起きる範囲
	const float kKnockBackStartRange = 10.0f;

	/// パーティクル
	// 発生個数
	const float kNumDeadParticle = 64;
	const float kNumDamageParticle = 64;
	const float kNumSpawnParticle = 16;

protected:
	std::vector<LWP::Primitive::Mesh> models_;

	// 追従カメラのアドレスを受け取る
	FollowCamera* followCamera_;

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
	// ノックバックフラグ(今は居合終了時のノックバックのみ)
	bool isKnockBack_;

	// ノックバックする方向ベクトル
	lwp::Vector3 knockBackDir_;

	// 死んだ時用アニメ
	MotionWork deadAnime = {
		// 下に沈んでいく速度
		.speed = 0.5f,
	};
	LWP::Resource::Motion deadMotion_;
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

	// 何かしらの判定に当たった(当たったことを検知)
	//bool wasHitCollision_ = false;

	// 無量空処かどうか
	bool isUtopia_ = false;

	// 無敵判定中ですか？
	bool isInvincible_ = false;

	// 硬直、内部の処理が進行しない時間
	// 当たった攻撃とか、場合や種類によって時間を設定する
	// この時間が 0 になるまで動けない
	float utopiaTime_ = 0.0f;

	// 攻撃に当たった後に少しだけ攻撃を受け付けない時間
	// 持続する系とかもあるかもだから一定の値を入れておきたい
	// 設定とかで固定値?
	// 0 じゃないと無敵(別でフラグを立てとく)
	float invincibleTime_ = 0.0f;

	// ノックバックの移動時間
	float knockBackTime_ = 0.0f;

	// パーティクル
	// ダメージを受けたとき
	static std::function<void(int, lwp::Vector3)> damageEffect_;
	// 死ぬとき
	static std::function<void(int, lwp::Vector3)> deadEffect_;
	// 出現したときのパーティクル
	static std::function<void(int, lwp::Vector3)> spawnEffect_;
	// 出現時の光の柱
	LWP::Primitive::Billboard3D lightPillar_;
	LWP::Resource::Motion lightPillarMotion_;
};