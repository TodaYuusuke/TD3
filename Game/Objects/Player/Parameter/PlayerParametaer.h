#pragma once


#include <Adapter.h>
#include "Game/Objects/Upgrade/IUpgrade.h"
#include "Game/Objects/Player/Config/PlayerConfing.h"
#include "Game/Objects/Player/HP/PlayerHP.h"

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
	/// パラメータ自体が持っている変数を更新する
	/// </summary>
	void Update();

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

	/// <summary>
	/// パラメーターを取得
	/// </summary>
	UpgradeParameter GetParameter() {
		return param;
	};

	/// <summary>
	/// HP を増やす
	/// </summary>
	void IncreaseHP();
	/// <summary>
	/// HP を減らす
	/// </summary>
	bool DecreaseHP();

	/// <summary>
	/// 最大 HP を増やす
	/// </summary>
	void IncreaseHPMAX();
	/// <summary>
	/// 最大 HP を減らす
	/// </summary>
	void DecreaseHPMAX();

public: //*** オペレーターオーバーロード ***//

	PlayerParameter operator*(const PlayerParameter& obj);

public:	//*** サブクラス ***//

	/// <summary>
	/// HP に関するパラメータ
	/// </summary>
	//struct HPParam
	//{
	//	//int hp_ = 0;	// 今のHP
	//	int maxHP_ = 0;	// 最大HP
	//};

	/// <summary>
	/// 攻撃自体に対してのパラメータ
	/// </summary>
	struct AttackParam
	{
		// 抜刀そのもの
		int slashNum_ = 0;			// 回数
		float slashPower_ = 0.0f;	// 強さ
		float slashRange_ = 0.0f;	// カプセル範囲
		float slashLength_ = 0.0f;	// スラッシュ距離補正

		AttackParam operator*(const AttackParam& obj)
		{
			AttackParam temp;
			temp.slashNum_ = std::max<int>(this->slashNum_, obj.slashNum_);
			temp.slashPower_ = this->slashPower_ * obj.slashPower_;
			temp.slashRange_ = this->slashRange_ * obj.slashRange_;
			temp.slashLength_ = this->slashLength_ * obj.slashLength_;
			return temp;
		}
	};

	/// <summary>
	/// 移動速度のみのパラメータ
	/// </summary>
	struct SpeedParam
	{
		float move_ = 0.0f;
		float slash_ = 0.0f;
		float moment_ = 0.0f;

		// 実際には代入されない
		float all_ = 0.0f;

		SpeedParam operator*(const SpeedParam& obj)
		{
			SpeedParam temp;
			temp.move_ = (this->move_ + this->all_) * (obj.move_ * obj.all_);
			temp.slash_ = (this->slash_ + this->all_) * (obj.slash_ * obj.all_);
			temp.moment_ = (this->moment_ + this->all_) * (obj.moment_ * obj.all_);
			return temp;
		}
	};

	/// <summary>
	/// 時間のみのパラメータ
	/// </summary>
	struct TimeParam
	{
		float justTake_;			// ジャスト判定の時間
		float momentTime_;			// 後隙自体の時間
		float invincibleDamage_;	// ダメージを受けた時の無敵

		TimeParam operator*(const TimeParam& obj)
		{
			TimeParam temp;
			temp.justTake_ = this->justTake_ * obj.justTake_;
			temp.momentTime_ = this->momentTime_ * obj.momentTime_;
			temp.invincibleDamage_ = this->invincibleDamage_ * obj.invincibleDamage_;
			return temp;
		}
	};

public:	//*** パブリック変数 ***//

	// HP のみ
	//HPParam Hp;
	PlayerHP Hp;

	// 攻撃の処理に関する
	AttackParam Attack;

	// 移動速度のみ
	SpeedParam Speed;

	// 時間のみ
	TimeParam Time;

private: //*** プライベート変数 ***//

	PlayerConfig* config_ = nullptr;

	UpgradeParameter param;

private: //*** プライベート関数 ***//

	void ApplyHP();
	void ApplyAttack();
	void ApplySpeed();
	void ApplyTime();
};