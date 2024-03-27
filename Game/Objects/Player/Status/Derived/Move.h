#pragma once
#include "../IStatus.h"

/// <summary>
/// 移動してるときの動き
/// </summary>
class Move : public IStatus
{
public:	// ** パブリックなメンバ変数 ** //

	/// <summary>
	/// 初期化(アニメーションごとの値を設定する)
	/// <para>最初に一回のみ呼び出す</para>
	/// </summary>
	void Init(Player* p) override;

	/// <summary>
	/// アニメーションを開始する関数
	/// </summary>
	void Reset() override;

	/// <summary>
	/// 更新(アニメーションさせる)
	/// <para>内部でプレイヤーからパラメータを受け取って動作させる</para>
	/// </summary>
	void Update() override;

private: //*** プライベート変数 ***//



private: //*** プライベート関数 ***//

	/// <summary>
	/// アニメーションを作成する
	/// </summary>
	void CreateMotions() override;

};