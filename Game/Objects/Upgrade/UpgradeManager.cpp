#include "UpgradeManager.h"

#include "Game/Objects/Player/Player.h"
#pragma region Upgrades
#include "Game/Objects/Upgrade/Derved/PowerUp.h"
#include "Game/Objects/Upgrade/Derved/PowerPerUp10.h"
#include "Game/Objects/Upgrade/Derved/PowerPerUp30.h"
#pragma endregion

using namespace LWP;
using namespace LWP::Math;


// 実体宣言
std::vector<L::IUpgrade*> L::UpgradeManager::upgrades_;
bool L::UpgradeManager::isLevelUpping;
int L::UpgradeManager::kUpgradNum_;
std::vector<int> L::UpgradeManager::candidata_;
int L::UpgradeManager::upgradedConut_;

void L::UpgradeManager::Init()
{
	// フラグ初期化
	isLevelUpping = false;

	upgrades_.clear();
	// アップグレードをすべて取得
	upgrades_.push_back(new PowerUp);
	upgrades_.push_back(new PowerUp);
	upgrades_.push_back(new PowerUp);
	upgrades_.push_back(new PowerUp);
	upgrades_.push_back(new PowerUp);
	upgrades_.push_back(new PowerPerUp10);
	upgrades_.push_back(new PowerPerUp30);

	// すべてを初期化する
	for (size_t i = 0; i < upgrades_.size(); i++)
	{
		upgrades_[i]->Init();
	}
	// アップグレード回数を初期化
	upgradedConut_ = 0;
	kUpgradNum_ = 3;
}

void L::UpgradeManager::Update(Player* player)
{
	// 新規で取得するアップグレードを選択する
	Selecting(player);
}

void L::UpgradeManager::LevelUp()
{
	isLevelUpping = true;
	RandomUpgrade();
}

void L::UpgradeManager::DebugWindow()
{
	ImGui::Begin("UpgradeManager");

	if (ImGui::Checkbox("isLevelUpFlag", &isLevelUpping) &&
		isLevelUpping == true)
	{
		// 押された瞬間
		LevelUp();
	}
	ImGui::Separator();

	ImGui::Text("upgrade: %d", kUpgradNum_);
	ImGui::Text("cursor : %d", cursorIndex_);
	ImGui::Text("choice : %d", choiceIndex_);
	ImGui::Separator();


	ImGui::Text("Nums : %d", upgrades_.size());
	if (ImGui::TreeNode("All"))
	{

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100));

		for (size_t i = 0; i < upgrades_.size(); i++)
		{
			ImGui::Text("upgrade : %d", i);
		}

		ImGui::EndChild();
		ImGui::TreePop();
		ImGui::Separator();
	}
	ImGui::Text("Nums : %d", upgradedConut_);
	if (ImGui::TreeNode("Selected"))
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100));

		for (size_t i = 0; i < upgrades_.size(); i++)
		{
			if (upgrades_[i]->isApplied)
			{
				ImGui::Text("upgrade : %d", i);
			}
		}

		ImGui::EndChild();
		ImGui::TreePop();
		ImGui::Separator();
	}

	ImGui::End();
}

void L::UpgradeManager::RandomUpgrade()
{
	candidata_.clear();
	while (candidata_.size() < kUpgradNum_)
	{
		// アップグレードの数が足りなくなったら
		if (upgrades_.size() - upgradedConut_ <= kUpgradNum_)
		{
			kUpgradNum_ = int(upgrades_.size()) - upgradedConut_ - 1;
		}
		// 取得する範囲の添え字を受け取る
		int rand = Utility::GenerateRandamNum(0, (int)upgrades_.size() - 1);
		// 判断
		bool isFind = false;
		// 既に選択しているものは出ない
		isFind = upgrades_[rand]->isApplied;

		// 被りが発生しても無理
		for (size_t i = 0; i < candidata_.size(); i++)
		{
			if (candidata_[i] == rand)
			{
				isFind = true;
				break;
			}
		}
		// 重なっていたらもう一度抽選
		if (isFind)
		{
			continue;
		}

		candidata_.push_back(rand);
	}
}

void L::UpgradeManager::Selecting(Player* player)
{
	// 場所
	Vector2 pos{ 0.0f,100.0f };
	for (size_t i = 0; i < kUpgradNum_; i++)
	{
		pos.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 1) * (i + 1);
		// 抽選されたアップグレードを更新
		upgrades_[candidata_[i]]->Update();
		upgrades_[candidata_[i]]->ShowUI(pos);
	}

	// カーソル移動
	if (0 < cursorIndex_ &&
		(Input::Keyboard::GetTrigger(DIK_A) ||
			Input::Keyboard::GetTrigger(DIK_LEFT) ||
			Input::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT)))
	{
		cursorIndex_--;
	}
	// 最後の添え字は決定
	if (cursorIndex_ < kUpgradNum_ &&
		(Input::Keyboard::GetTrigger(DIK_D) ||
			Input::Keyboard::GetTrigger(DIK_RIGHT) ||
			Input::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT)))
	{
		cursorIndex_++;
	}
	// 下ボタンを押したときは即座に決定ボタンへ
	if (Input::Keyboard::GetTrigger(DIK_S) ||
		Input::Keyboard::GetTrigger(DIK_DOWN) ||
		Input::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		cursorIndex_ = kUpgradNum_;
	}


	// 決定ボタンを押した
	if (Input::Keyboard::GetTrigger(DIK_SPACE) ||
		Input::Keyboard::GetTrigger(DIK_RETURN) ||
		Input::Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		// アップグレードを選択完了なら
		if (cursorIndex_ == kUpgradNum_)
		{
			// 選択されたアップグレードを適応する
			if (kUpgradNum_ != 0)
			{
				Selected();
			}
			// アップグレードが残ってなかったら適用のみ
			Apply(player);
			cursorIndex_ = 0;
			choiceIndex_ = 0;
			return;
		}
		// アップデートを仮選択
		choiceIndex_ = cursorIndex_;
	}

}

void L::UpgradeManager::Selected()
{
	// 選択されたアップグレードを適応させる
	upgrades_[candidata_[choiceIndex_]]->isApplied = true;
	upgradedConut_++;
}

void L::UpgradeManager::Apply(Player* player)
{
	// アップグレード内容を作る
	UpgradeParameter para;

	// すべて更新
	for (size_t i = 0; i < upgrades_.size(); i++)
	{
		// 描画を消す
		upgrades_[i]->Update();
		// 選択されているものだけ適応
		if (upgrades_[i]->isApplied)
		{
			upgrades_[i]->Apply(&para);
		}
	}
	// プレイヤーに適応
	player->ApplyUpgrade(para);
	// フラグを戻す
	isLevelUpping = false;
}