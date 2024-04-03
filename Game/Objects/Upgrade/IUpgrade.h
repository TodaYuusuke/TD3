#pragma once
#include <Adapter.h>

/// <summary>
/// ほぼすべてのアップグレードをこの型にしてもいいと思う
/// </summary>
struct Paramete
{
	// 基準(定数)
	float base = 0.0f;
	// 補正(％)
	float percent = 100.0f;
};

/// <summary>
/// アップグレードによって変化する値と倍率を司る
/// </summary>
struct UpgradeParameter {
	// 攻撃力
	Paramete slashPowerDelta;
	// 攻撃範囲
	Paramete slashRangeDelta;
	// 移動速度
	Paramete allSpeedDelta;
	// 攻撃回数 : 倍率は使わない
	Paramete slashDelta;
	// 体力 : 倍率は使わない
	Paramete hpDelta;
};

namespace L {
	/// <summary>
	/// アップグレード基底クラス（新しいアップグレードはこれを継承してつくること）
	/// </summary>
	class IUpgrade {
	public:	// ** パブリックなメンバ変数 ** //
		
		// 適応ずみかフラグ
		bool isApplied = false;


		/// <summary>
		/// アップグレードを適応する関数
		/// <para>最後にisAppliedを変更すること！</para>
		/// </summary>
		/// <param name="p"></param>
		virtual void Apply(UpgradeParameter* para) = 0;

		/// <summary>
		/// UI用のテクスチャのパスを返す関数
		/// </summary>
		/// <returns>resources/texture/以降のパス</returns>
		virtual std::string GetTexturePass() = 0;
		
		/// <summary>
		/// UI用のテクスチャのパスを返す関数
		/// </summary>
		/// <returns>resources/texture/以降のパス</returns>
		virtual std::string GetUpgradeName() = 0;

		/// <summary>
		/// 初期化（sprite用テクスチャ読み込み）
		/// </summary>
		void Init();

		/// <summary>
		/// 更新（アニメーションとかあるかもなので用意）
		/// <para>必ずSpriteのisActiveをfalseに！</para>
		/// <para>特殊なアニメーションとかしたいなら継承</para>
		/// </summary>
		virtual void Update();

		/// <summary>
		/// スプライト表示
		/// <para>この関数を呼び出さないとそのフレームは表示されない（SpriteのisActiveをtrueにしてね）</para>
		/// </summary>
		/// <param name="pos"></param>
		void ShowUI(const LWP::Math::Vector2& pos);


	protected: // ** 派生先用のメンバ変数 ** //

		// スプライト
		LWP::Primitive::Sprite sprite_;
	};
}
