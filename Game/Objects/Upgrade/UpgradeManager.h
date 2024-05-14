#pragma once
#include "ISkill.h"
#include <Adapter.h>

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
		void Init(LWP::Object::Camera* cameraptr);
		
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

//#ifdef DEMO
		void DebugWindow(Player* player);
//#endif // DEMO

	private: // ** メンバ変数 ** //

		// アップグレードのリスト
		static std::vector<ISkill*> attackUpgrades_;
		static std::vector<ISkill*> escapeUpgrades_;

		// レベルアップ中フラグ
		static bool isLevelUpping;

		// 表示するアップグレードの数
		static int kUpgradNum_;

		// ランダムで生成した n つのアップグレード
		static std::vector<int> candidata_;

		// カーソルを合わせているアップグレード(何番目か)
		int cursorIndex_ = 0;

		// 選択しているアップグレード
		// 0 : 攻撃, 1 : hoge
		int choiceIndex_ = 0;

		// アップグレードを取得した回数
		static int upgradedConut_;

		// 選択場所のわかるUI
		LWP::Primitive::Sprite sprite_;

		lwp::Vector3 cursorPos = { LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2),120.0f };

		// 長押し用のフラグ
		bool isPress_ = false;

		// 長押しした時間
		float pressTime_ = 0.0f;
		// 決定に必要な時間
		const float kPressTime_ = 1.0f;

	private: // プライベート関数

		/// <summary>
		/// アップグレードを適応していないものから取り出す
		/// <para>レベルアップしたときに抽選する</para>
		/// </summary>
		static void RandomUpgrade();

		/// <summary>
		/// リストから要素を取得
		/// </summary>
		/// <param name="f">true : 攻撃, false : 逃走</param>
		/// <returns></returns>
		static int ChooseOnce(bool f);

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

		// カーソルのパーティクル
		void CursorParticleInit();
		// パーティクル
		static std::function<void(int, lwp::Vector3)> CursorEffect_;

		LWP::Object::Camera* mainCameraptr_;
		/*LWP::Base::WinApp* winAppPtr_;*/
	};
}
lwp::Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);