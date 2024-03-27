#pragma once
#include <Adapter.h>


// 前方宣言
class Player;

/// <summary>
/// アニメーションステータス基底クラス
/// </summary>
class IStatus
{
public:	// ** パブリックなメンバ変数 ** //

	/// <summary>
	/// プレイヤー行動
	/// </summary>
	enum class Behavior : size_t
	{
		Root = 0u,	// 待機状態
		Move,		// 移動
		Slash,		// 居合攻撃
		Moment,		// とりあえず居合の後隙
		Damage,		// 攻撃を受けた時

		_COUNT,		// 状態最大数 : 使用禁止
	};
	/// <summary>
	/// 初期化(アニメーションごとの値を設定する)
	/// <para>最初に一回のみ呼び出す</para>
	/// </summary>
	virtual void Init(Player* p);

	/// <summary>
	/// アニメーションを開始する関数
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// 更新(アニメーションさせる)
	/// <para>内部でプレイヤーからパラメータを受け取って動作させる</para>
	/// <para>当たり判定とか行動の結果によるフラグを管理</para>
	/// </summary>
	virtual void Update();

protected: //*** プロテクト関数 ***//

	/// <summary>
	/// アニメーションを作成する
	/// <para>アニメーションは順次追加する</para>
	/// </summary>
	virtual void CreateMotions() = 0;

	/// <summary>
	/// 操作入力を受け取る
	/// </summary>
	void CheckInputMove();

protected: // ** 派生先用のメンバ変数 ** //

	// プレイヤーのポインタ
	Player* player_ = nullptr;


	// 状態内で動くアニメーション
	std::vector<LWP::Resource::Motion> motions_;

	// 向いている方向
	LWP::Math::Vector3 destinate_ = { 0.0f,0.0f,1.0f };

	// 移動入力があったか
	bool isInputMove_ = false;

	// この行動に入ってからの時間
	float elapsedTime_ = 0.0f;
	// この行動が終わる時間
	float EndTime_ = 0.0f;

};