#pragma once
#include "ISkill.h"

// 前方宣言
class Player;

namespace L {
	/// <summary>
	/// アップグレードを管理するクラス
	/// </summary>
	class UpgradeManager {
	public: // ** メンバ関数 ** //

		/// <summary>
		/// 初期化（シーンの最初のみで問題なし）
		/// <para>upgrades_に継承したクラスのインスタンスをpush_backしてね</para>
		/// </summary>
		void Init();
		
		/// <summary>
		/// 更新
		/// <para>この更新はレベルアップ中（選択待ち）のみ呼び出すこと</para>
		/// </summary>
		void Update(Player* player);

		/// <summary>
		/// レベルアップしたらこの関数を呼ぶこと！
		/// </summary>
		static void LevelUp();
		/// <summary>
		/// レベルアップ中フラグを返す（シーンを止めるため）
		/// </summary>
		static bool GetLevelUpFlag() { return isLevelUpping; }

#ifdef DEMO
		void DebugWindow(Player* player);
#endif // DEMO

	private: // ** メンバ変数 ** //

		// アップグレードのリスト
		static std::vector<ISkill*> upgrades_;

		// レベルアップ中フラグ
		static bool isLevelUpping;

		// 表示するアップグレードの数
		static int kUpgradNum_;

		// ランダムで生成した三つのアップグレード
		static std::vector<int> candidata_;

		// カーソルを合わせているアップグレード(何番目か)
		int cursorIndex_ = 0;

		// 選択しているアップグレード(何番目か)
		int choiceIndex_ = 0;

		// アップグレードを取得した回数
		static int upgradedConut_;

		// プレイヤーの HP を監視
		//UpgradeParameter preParam_;

	private: // プライベート関数

		/// <summary>
		/// アップグレードを適応していないものから取り出す
		/// <para>レベルアップしたときに抽選する</para>
		/// </summary>
		static void RandomUpgrade();

		/// <summary>
		/// アップグレードを選択中に呼び出す関数
		/// </summary>
		void Selecting(Player* player);

		/// <summary>
		/// アップグレードを選択完了した時に呼び出す関数
		/// </summary>
		void Selected();

		/// <summary>
		/// アップグレードを
		/// </summary>
		void Apply(Player* player);

	};
}
