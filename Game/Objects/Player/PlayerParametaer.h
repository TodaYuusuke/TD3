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

public: //*** オペレーターオーバーロード ***//

	const PlayerParameter& operator*(const PlayerParameter& obj);

public:	//*** サブクラス ***//

	/// <summary>
	/// HP に関するパラメータ
	/// </summary>
	struct HP
	{
		int hp_ = 0;	// 今のHP
		int maxHP_ = 0;	// 最大HP
	};

	/// <summary>
	/// 攻撃自体に対してのパラメータ
	/// </summary>
	struct Attack
	{
		// 抜刀そのもの
		int slashNum_ = 0;			// 回数
		float slashPower_ = 0.0f;	// 強さ
		float slashRange_ = 0.0f;	// カプセル範囲
	};

	/// <summary>
	/// 移動速度のみのパラメータ
	/// </summary>
	struct Speed
	{
		float move_ = 0.0f;
		float slash_ = 0.0f;
		float moment_ = 0.0f;
	};

public:	//*** パブリック変数 ***//
	// 体力
	int hp_ = 0;
	// 攻撃力
	float slashPower_ = 0.0f;
	// 攻撃範囲
	float slashRange_ = 0.0f;
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