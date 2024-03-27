#pragma once
#include <Adapter.h>

// 前方宣言
class Player;

/// <summary>
/// モーションステータス基底クラス
/// </summary>
class IStatus
{
public:	// ** パブリックなメンバ変数 ** //

	~IStatus() = default;

	/// <summary>
	/// 初期化(モーションごとの値を設定する)
	/// <para>最初に一回のみ呼び出す</para>
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新(モーションさせる)
	/// <para>内部でプレイヤーからパラメータを受け取って動作させる</para>
	/// </summary>
	virtual void Update(Player* p);

protected: // ** 派生先用のメンバ変数 ** //

};