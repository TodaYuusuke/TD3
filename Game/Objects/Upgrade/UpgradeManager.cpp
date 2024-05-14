#include "UpgradeManager.h"

#include "Game/Objects/Player/Player.h"
#include "Game/Objects/GameTimer/GameTimer.h"
#pragma region Upgrades
#include "Game/Objects/Upgrade/SkDerved/SkillList.h"
#pragma endregion

using namespace LWP;
using namespace LWP::Math;


// 実体宣言
std::vector<L::ISkill*> L::UpgradeManager::attackUpgrades_;
std::vector<L::ISkill*> L::UpgradeManager::escapeUpgrades_;
bool L::UpgradeManager::isLevelUpping;
int L::UpgradeManager::kUpgradNum_;
std::vector<int> L::UpgradeManager::candidata_;
int L::UpgradeManager::upgradedConut_;

void L::UpgradeManager::Init(LWP::Object::Camera* cameraptr)
{
	// フラグ初期化
	isLevelUpping = false;

	attackUpgrades_.clear();
	// アップグレードをすべて取得
	// 最大 4 つ
	// 攻撃
	attackUpgrades_.push_back(new Skill_PursuitFlag);
	attackUpgrades_.push_back(new Skill_PowerUp1);
	attackUpgrades_.push_back(new Skill_PowerUp1);
	attackUpgrades_.push_back(new Skill_AttackRangeUp);

	// 逃走
	escapeUpgrades_.push_back(new Skill_DamageInvincibleAdd);
	escapeUpgrades_.push_back(new Skill_EXLifeFlag);
	escapeUpgrades_.push_back(new Skill_BlowOffFlag);
	escapeUpgrades_.push_back(new Skill_AttackLengthUp);

	// すべてを初期化する
	for (size_t i = 0; i < attackUpgrades_.size(); i++)
	{
		attackUpgrades_[i]->Init();
	}

	// すべてを初期化する
	for (size_t i = 0; i < escapeUpgrades_.size(); i++)
	{
		escapeUpgrades_[i]->Init();
	}
	// アップグレード回数を初期化
	upgradedConut_ = 0;
	kUpgradNum_ = 1;

	sprite_.texture = LWP::Resource::LoadTexture("cursor.png");
	sprite_.anchorPoint = { 0.5f,0.5f };
	sprite_.transform.translation = cursorPos;
	sprite_.isUI = true;
	sprite_.isActive = false;

	mainCameraptr_ = cameraptr;

	CursorParticleInit();
}

void L::UpgradeManager::Update(Player* player)
{
	// 新規で取得するアップグレードを選択する
	Selecting(player);
}

void L::UpgradeManager::LevelUp()
{
	GameTimer::GetInstance()->Stop();

	isLevelUpping = true;
	RandomUpgrade();
}
//#ifdef  DEMO
void L::UpgradeManager::DebugWindow(Player* player)
{
#ifdef DEMO

	ImGui::Begin("UpgradeManager");


	if (ImGui::Button("isLevelUpFlag") &&
		isLevelUpping == false)
	{
		isLevelUpping = true;
		// 押された瞬間
		LevelUp();
	}
	if (ImGui::Button("ReApply"))
	{
		Apply(player);
	}
	ImGui::Separator();

	ImGui::Text("upgrade: %d", kUpgradNum_);
	ImGui::Text("cursor : %d", cursorIndex_);
	ImGui::Text("choice : %d", choiceIndex_);
	ImGui::Separator();

	ImGui::Text("ChoseUpgrade : %d", candidata_.size());
	//for (size_t i = 0; i < candidata_.size(); i++)
	if (!candidata_.empty())
	{
		attackUpgrades_[candidata_[0]]->DebugTree();
		escapeUpgrades_[candidata_[1]]->DebugTree();
	}

	ImGui::Separator();
	ImGui::Text("Nums : %d", attackUpgrades_.size());
	if (ImGui::TreeNode("All"))
	{

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100));

		for (size_t i = 0; i < attackUpgrades_.size(); i++)
		{
			attackUpgrades_[i]->DebugTree();
		}

		ImGui::EndChild();
		ImGui::TreePop();
		ImGui::Separator();
	}
	ImGui::Text("Nums : %d", upgradedConut_);
	if (ImGui::TreeNode("Selected"))
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 70));

		for (size_t i = 0; i < attackUpgrades_.size(); i++)
		{
			if (attackUpgrades_[i]->isApplied)
			{
				attackUpgrades_[i]->DebugTree();
			}
		}
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 70));

		for (size_t i = 0; i < escapeUpgrades_.size(); i++)
		{
			if (escapeUpgrades_[i]->isApplied)
			{
				escapeUpgrades_[i]->DebugTree();
			}
		}

		ImGui::EndChild();
		ImGui::TreePop();
		ImGui::Separator();
	}

	ImGui::End();
#endif // DEMO
}
//#endif //  DEMO

void L::UpgradeManager::RandomUpgrade()
{
	candidata_.clear();

	// 攻撃要素を取得
	candidata_.push_back(ChooseOnce(true));

	// 逃走要素を取得
	candidata_.push_back(ChooseOnce(false));

}

int L::UpgradeManager::ChooseOnce(bool f)
{
	// 返す値
	int result = 0;
	// 攻撃
	if (f)
	{
		// 抽選候補に入れる
		while (1)
		{
			// 取得する範囲の添え字を受け取る
			// 同じ数だけ作れば問題なし
			int rand = Utility::GenerateRandamNum(0, (int)attackUpgrades_.size() - 1);
			// 判断
			bool isFind = false;
			// 既に選択しているものは出ない
			isFind = attackUpgrades_[rand]->isApplied;

			// 被りが発生しても無理
			//for (size_t i = 0; i < candidata_.size(); i++)
			/*{
				if (!candidata_.empty() && candidata_[0] == rand)
				{
					isFind = true;
					break;
				}
			}*/
			// 重なっていたらもう一度抽選
			if (isFind)
			{
				continue;
			}
			return rand;
		}
	}
	// hoge
	else
	{
		// 抽選候補に入れる
		while (1)
		{
			// 取得する範囲の添え字を受け取る
			// 同じ数だけ作れば問題なし
			int rand = Utility::GenerateRandamNum(0, (int)escapeUpgrades_.size() - 1);
			// 判断
			bool isFind = false;
			// 既に選択しているものは出ない
			isFind = escapeUpgrades_[rand]->isApplied;

			// 被りが発生しても無理
			//for (size_t i = 0; i < candidata_.size(); i++)
			/*{
				if (candidata_.size() == 2 && candidata_[1] == rand)
				{
					isFind = true;
					break;
				}
			}*/
			// 重なっていたらもう一度抽選
			if (isFind)
			{
				continue;
			}
			return rand;
		}
	}
}

void L::UpgradeManager::Selecting(Player* player)
{
	// 場所
	Vector2 pos{ 0.0f,600.0f };
	//sprite_.isActive = true;
	pos.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2);
	// 抽選されたアップグレードを更新
	attackUpgrades_[candidata_[0]]->Update();
	attackUpgrades_[candidata_[0]]->ShowUI(pos);
	// 
	pos.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2) * 2;
	// 抽選されたアップグレードを更新
	escapeUpgrades_[candidata_[1]]->Update();
	escapeUpgrades_[candidata_[1]]->ShowUI(pos);

	// カーソルUI
	sprite_.transform.translation.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2) * (cursorIndex_ + 1);

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

	// 決定ボタンを押した
	if (Input::Keyboard::GetPress(DIK_SPACE) ||
		Input::Keyboard::GetPress(DIK_RETURN) ||
		Input::Pad::GetPress(XINPUT_GAMEPAD_A))
	{
		isPress_ = true;	
		// AL3_02_15
		// ビューポート行列
		// サイズ　1920 x 1080
		//HWND hwnd = GetHWND();
		//ScreenToClient(LWP::Base::WinApp::GetHWND(), lwp::Vector2{ sprite_.transform.translation.x,sprite_.transform.translation.y });
		lwp::Matrix4x4 matViewport = MakeViewportMatrix(0, 0,1980, 1080, 0, 1);
		lwp::Matrix4x4 viewProj = mainCameraptr_->GetViewProjection();
		lwp::Matrix4x4 matVPV = matViewport * viewProj;
		matVPV = matVPV.Inverse();

		lwp::Vector3 posNear = Vector3(sprite_.transform.translation.x, sprite_.transform.translation.y,0.1f);
		lwp::Vector3 posFar = Vector3(sprite_.transform.translation.x, sprite_.transform.translation.y,1);

		posNear = lwp::Matrix4x4::TransformCoord(posNear,matVPV);
		posFar = lwp::Matrix4x4::TransformCoord(posFar,matVPV);
		lwp::Vector3 direction = posFar - posNear;
		direction = direction.Normalize();
		direction = posNear + direction * 1.0f;
		CursorEffect_(16, direction);
	}
	else
	{
		isPress_ = false;
		pressTime_ = 0.0f;
	}
	if (isPress_)
	{
		pressTime_ += lwp::GetDefaultDeltaTimeF();



		if (kPressTime_ <= pressTime_)
		{
			choiceIndex_ = cursorIndex_;
			// 選択されたアップグレードを適応する
			if (kUpgradNum_ != 0)
			{
				Selected();
			}
			// アップグレードが残ってなかったら適用のみ
			Apply(player);
			cursorIndex_ = 0;
			choiceIndex_ = 0;
		}
	}

}

void L::UpgradeManager::Selected()
{
	// 選択されたアップグレードを適応させる
	if (choiceIndex_ == 0)
		attackUpgrades_[candidata_[choiceIndex_]]->isApplied = true;
	else
		escapeUpgrades_[candidata_[choiceIndex_]]->isApplied = true;
	upgradedConut_++;
	sprite_.isActive = false;
}

void L::UpgradeManager::Apply(Player* player)
{
	// アップグレード内容を作る
	UpgradeParameter para;

	// すべて更新
	for (size_t i = 0; i < attackUpgrades_.size(); i++)
	{
		// 描画を消す
		attackUpgrades_[i]->Update();
		// 選択されているものだけ適応
		if (attackUpgrades_[i]->isApplied)
		{
			attackUpgrades_[i]->Apply(&para);
		}
	}
	// すべて更新
	for (size_t i = 0; i < escapeUpgrades_.size(); i++)
	{
		// 描画を消す
		escapeUpgrades_[i]->Update();
		// 選択されているものだけ適応
		if (escapeUpgrades_[i]->isApplied)
		{
			escapeUpgrades_[i]->Apply(&para);
		}
	}
	// プレイヤーに適応
	player->ApplyUpgrade(para);
	// フラグを戻す
	isLevelUpping = false;

	//// 情報を保存
	//preParam_ = para;
	GameTimer::GetInstance()->Start();
}

void L::UpgradeManager::CursorParticleInit()
{
	static LWP::Object::Particle CursorParticle_;
	CursorParticle_.SetPrimitive<Primitive::Cube>();
	CursorParticle_.P()->transform.scale = { 0.0001f,0.001f, 0.0001f };
	CursorParticle_.P()->material.enableLighting = true;
	CursorParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::RED);
	CursorParticle_.initFunction = [](Primitive::IPrimitive* primitive)
		{
			Object::ParticleData newData{};
			newData.wtf.translation = lwp::Vector3{ 0,1,0 } + primitive->transform.GetWorldPosition();
			newData.wtf.rotation = primitive->transform.rotation;
			newData.wtf.scale = { 0.25f,0.25f, 0.25f };

			// 速度ベクトルを生成
			int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
			int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
			int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
			// 発射のベクトル
			Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
			newData.velocity = dir.Normalize() * 0.3f;

			// パーティクル追加
			return newData;
		};
	CursorParticle_.updateFunction = [](Object::ParticleData* data)
		{
			// 経過フレーム追加
			data->elapsedFrame++;

			data->wtf.translation += data->velocity;    // 速度ベクトルを加算
			data->wtf.rotation += data->velocity;    // ついでに回転させとく
			data->wtf.translation.y += -9.8f / 80.0f;    // 重力を加算

			// 速度ベクトルを弱める
			data->velocity *= 0.9f;

			return data->elapsedFrame > 180 ? true : false;
		};
	CursorParticle_.isActive = true;

	CursorEffect_ = [&](int i, lwp::Vector3 pos)
		{
			
			CursorParticle_.P()->transform = pos;
			CursorParticle_.Add(i);
		};
}

lwp::Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	lwp::Matrix4x4 result = {
	width / 2,0,0,0,
	0,-(height / 2),0,0,
	0,0,maxDepth - minDepth,0,
	left + (width / 2),top + (height / 2),minDepth,1
	};
	return result;
}

std::function<void(int, lwp::Vector3)> L::UpgradeManager::CursorEffect_ = nullptr;