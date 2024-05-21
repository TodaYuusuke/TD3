#pragma once
#include <Adapter.h>

#include "UpDerved/UpgradeList.h"

namespace L
{
	/// <summary>
	/// アップグレードスキル基底クラス（新しいアップグレードスキルはこれを継承してつくること）
	/// アップグレードを複数まとめて持つ
	/// </summary>
	class ISkill
	{
	public:	// ** パブリックなメンバ変数 ** //

		// 適応ずみかフラグ
		bool isApplied = false;
		// リストにまとめて複数対応
		std::vector<IUpgrade*> attackUpgrades_;

		/// <summary>
		/// アップグレード数を返す
		/// </summary>
		/// <returns>配列数</returns>
		size_t GetUpgradeNum() const
		{
			return attackUpgrades_.size();
		}

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
		/// デバッグ表示で Tree を作る関数
		/// </summary>
		virtual void DebugTree();

		/// <summary>
		/// アップグレードスキルの名前を返す関数
		/// </summary>
		/// <returns>TreeNode の名前になる</returns>
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
		void BaseUpdate();
		virtual void Update() {}

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
