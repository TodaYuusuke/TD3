#pragma once
#include <Adapter.h>

struct UpgradeParameter {
	// 攻撃力(定数)
	float power = 1.0f;
	// 攻撃力(％)
	float powerPer = 100.0f;
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
		LWP::Primitive::Sprite* sprite_;
	};
}
