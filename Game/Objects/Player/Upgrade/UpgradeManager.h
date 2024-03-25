#pragma once
#include "IUpgrade.h"

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
		/// <para>upgrades_に継承したクラスのインスタンスを１つずつpush_backしてね</para>
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
		static void LevelUp() { isLevelUpping = true; }
		/// <summary>
		/// レベルアップ中フラグを返す（シーンを止めるため）
		/// </summary>
		static bool GetLevelUpFlag() { return isLevelUpping; }


	private: // ** メンバ変数 ** //

		// アップグレードのリスト
		std::vector<IUpgrade*> upgrades_;

		// レベルアップ中フラグ
		static bool isLevelUpping;
	};
}
