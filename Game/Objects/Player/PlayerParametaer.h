#pragma once


#include <Adapter.h>
#include "Game/Objects/Upgrade/IUpgrade.h"
#include "PlayerConfing.h"

/// <summary>
/// プレイヤーの持つパラメーターをまとめたクラス
/// </summary>
class PlayerParameter
{
public:	//*** パブリック関数 ***//

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="p">コンフィグを取得</param>
	void Initialize(PlayerConfig* p);

	/// <summary>
	/// アップグレードを適応するための関数
	/// <para>UpgradeManagerがアップグレードの選択確定時に呼び出します</para>
	/// </summary>
	/// <param name="para">レベルアップによって変わったパラメーター</param>
	void ApplyUpgrade();
	void ApplyUpgrade(const UpgradeParameter& para);

	/// <summary>
	/// パラメーターを初期値にする
	/// </summary>
	void ResetParameter();


public:	//*** パブリック変数 ***//
	// 体力
	int hp_ = 0;
	// 攻撃力
	float power_ = 0.0f;
	// 攻撃範囲
	float attackRange_ = 0.0f;
	// 通常移動速度
	float moveSpeed = 0.0f;
	// 居合移動速度
	float slashSpeed = 0.0f;
	// 後隙移動速度
	float momentSpeed = 0.0f;
	// 居合回数
	int slashNum = 0;

private: //*** プライベート変数 ***//

	PlayerConfig* config_ = nullptr;

	UpgradeParameter param;
};