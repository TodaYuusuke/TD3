#pragma once

#include <Adapter.h>

class PlayerConfig
{
public: //*** パブリック関数 ***//

	void Initialize();

public: //*** サブクラス ***//

	// 移動距離 : (1 秒間に進む距離)
	struct Speeds
	{
		// プレイヤーの通常移動
		// 移動する距離
		float MOVE_;
		// プレイヤーの居合
		// 移動する距離
		float SLASH_;
		// プレイヤーの後隙
		// 移動する距離
		float MOMENT_;
	};

	// 秒時間
	struct Times
	{
		// 各状態にかかる時間
		float ROOTBASE_;
		float MOVEBASE_;
		float SLASHBASE_;
		float MOMENTBASE_;
		float DAMAGEBASE_;

		// 居合による後隙の加算分
		float MOMENTINCREMENT_;
		// 居合攻撃が終わった後に加える無敵時間
		float SLASHENDINVINCIBLETIME_;
		// ジャスト居合を取る時間
		float JUSTTAKETIME_;
		// ジャスト抜刀成功時に加える無敵時間
		float JUSTINVINCIBLEADD_;
		// 被ダメージ時の無敵時間
		float DAMAGEINVINCIBLE_;
	};

	// 半径
	struct Lengths
	{
		// 武器の半径
		float WEAPONCOLLISIONRADIUS_;
		// 居合時の武器の前側への補正
		float WEAPONPLUSCORRECTION_;
		// ジャストの半径
		float JUSTCOLLISIONRADIUS_;
		// 敵の攻撃からの有効範囲
		//float JUSTENABLERANGE_ = 2.0f;
	};

	// カウント系
	struct Counts
	{
		// 居合回数の初期値
		uint32_t SLASHRELATIONBASE_;
		// 体力
		uint32_t BASEHP_;
		// 最大体力
		uint32_t MAXHP_;
		// 敵を何体倒すと HP 回復するか
		uint32_t UPGRADEPENETORATIONNUM_;
	};

	// 割合系
	struct Parcentages
	{
		// 居合攻撃からジャスト判定が取れる距離の割合
		float JUSTENABLE_;
	};

	// 強さ
	struct Power
	{
		// 初期攻撃力
		float BASEPOWER_;
		// アップグレード系
		// 追撃
		float BASEPURSUIT;
		// 継続ダメージ
		float BASEBURNING;
	};

	struct Other
	{
		// 経験値取得範囲
		float RADIUSLEVEL_;
	};

	struct Configs
	{
		Speeds Speed_;
		Times Time_;
		Lengths Length_;
		Counts Count_;
		Parcentages Par_;
		Power Power_;
	};


public: //*** パブリック変数 ***//

	Speeds Speed_;
	Times Time_;
	Lengths Length_;
	Counts Count_;
	Parcentages Parcent_;
	Power Power_;
	Other Other_;

private: //*** プライベート関数 ***//

	void InitSpeed();
	void InitTime();
	void InitLength();
	void InitCount();
	void InitParcentage();
	void InitPower();
	void InitOther();
};