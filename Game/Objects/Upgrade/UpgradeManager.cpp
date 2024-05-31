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


	// すべて削除
	for (size_t i = 0; i < attackUpgrades_.size(); i++)
	{
		delete attackUpgrades_[i];
	}

	// すべて削除
	for (size_t i = 0; i < escapeUpgrades_.size(); i++)
	{
		delete escapeUpgrades_[i];
	}
	attackUpgrades_.clear();
	escapeUpgrades_.clear();
#ifdef DEMO
	// アップグレードをすべて取得
	// 最大 4 つ
	// 攻撃
	attackUpgrades_.push_back(new Skill_PursuitFlag);
	attackUpgrades_.push_back(new Skill_PowerUp1);
	attackUpgrades_.push_back(new Skill_PowerUp2);
	attackUpgrades_.push_back(new Skill_PowerUp3);
	attackUpgrades_.push_back(new Skill_AttackRangeUp);
	attackUpgrades_.push_back(new Skill_BlowOffFlag);
	attackUpgrades_.push_back(new Skill_BurningFlag);

	// 逃走
	escapeUpgrades_.push_back(new Skill_DamageInvincibleAdd);
	escapeUpgrades_.push_back(new Skill_RadiusLevelUp);
	escapeUpgrades_.push_back(new Skill_MomentTimeDown1);
	escapeUpgrades_.push_back(new Skill_MomentTimeDown2);
	escapeUpgrades_.push_back(new Skill_EXLifeFlag);
	escapeUpgrades_.push_back(new Skill_PenetrationFlag);
	//取得していると他のアップグレードを取得したときにも適応される
	escapeUpgrades_.push_back(new Skill_HPUp);

#else
	// 製品版
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
#endif // DEMO

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

	sprite_.texture = LWP::Resource::LoadTexture("cursor2.png");
	sprite_.anchorPoint = { 0.5f,0.5f };
	sprite_.transform.translation = cursorPos;
	sprite_.isUI = true;
	sprite_.isActive = false;

	mainCameraptr_ = cameraptr;

	// 選択中のカーソルのモーション
	selectMotion_.Add(&sprite_.transform.scale, lwp::Vector3{ 0,0.5f,0 }, 0, 0.25f, LWP::Utility::Easing::Type::InQuart)
		.Add(&sprite_.transform.scale, lwp::Vector3{ 0.2f,0.0f,0 }, 0.1f, 0.15f, LWP::Utility::Easing::Type::InQuart)
		.Add(&sprite_.transform.scale, lwp::Vector3{ -0.2f,-0.5f,0 }, 0.25f, 0.15f, LWP::Utility::Easing::Type::OutQuart);

	// 画面中央へ行くために必要な移動量
	lwp::Vector3 centerVelocity = { 0,-70.0f,0 };
	// 選択後のアニメーション
	selectedMotion_.Add(&selectedAnimPos_, centerVelocity,
		0, 0.2f,
		LWP::Utility::Easing::Type::OutQuart)
		.Add(&selectedAnimPos_, lwp::Vector3{ -1000,0.0f,0 },
			0.5f, 0.3f,
			LWP::Utility::Easing::Type::InQuart);

	selectedMotion_.DisableDeltaTimeMultiply();

	// 選択中のパーティクル
	CursorParticleInit();

	//SE
	serectSE = std::make_unique<LWP::Resource::Audio>();
	serectSE->Load("fanfare.wav");
	chooseSE = std::make_unique<LWP::Resource::Audio>();
	chooseSE->Load("fanfare.wav");
}

void L::UpgradeManager::Update(Player* player)
{

	// すべて更新
	for (size_t i = 0; i < attackUpgrades_.size(); i++)
	{
		// 描画を消す
		attackUpgrades_[i]->BaseUpdate();
		// 選択されているものだけ適応
		if (attackUpgrades_[i]->isApplied)
		{
			attackUpgrades_[i]->BaseUpdate();
		}
	}
	// すべて更新
	for (size_t i = 0; i < escapeUpgrades_.size(); i++)
	{
		// 描画を消す
		escapeUpgrades_[i]->BaseUpdate();
		// 選択されているものだけ適応
		if (escapeUpgrades_[i]->isApplied)
		{
			escapeUpgrades_[i]->BaseUpdate();
		}
	}
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

	if (ImGui::Button("ReApply"))
	{
		Apply(player);
	}
	if (ImGui::Button("Random"))
	{
		RandomUpgrade();
	}
	ImGui::Separator();

	ImGui::Text("upgrade: %d", kUpgradNum_);
	ImGui::Text("cursor : %d", cursorIndex_);
	ImGui::Text("choice : %d", choiceIndex_);
	ImGui::Text("Attack : %d", attackUpgrades_.size());
	ImGui::Text("Escape : %d", escapeUpgrades_.size());
	ImGui::Separator();

	ImGui::Text("ChoseUpgrade : %d", candidata_.size());
	//for (size_t i = 0; i < candidata_.size(); i++)
	if (!candidata_.empty())
	{
		attackUpgrades_[candidata_[0]]->DebugTree();
		escapeUpgrades_[candidata_[1]]->DebugTree();
	}

	ImGui::Separator();
	ImGui::Text("AttackUpgrade : %d", attackUpgrades_.size());
	if (ImGui::TreeNode("Attack"))
	{
		//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100));

		for (size_t i = 0; i < attackUpgrades_.size(); i++)
		{
			attackUpgrades_[i]->DebugTree();
		}

		//ImGui::EndChild();
		ImGui::TreePop();
		ImGui::Separator();
	}
	ImGui::Separator();
	ImGui::Text("EscapeUpgrade : %d", attackUpgrades_.size());
	if (ImGui::TreeNode("Escape"))
	{
		//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100));

		for (size_t i = 0; i < escapeUpgrades_.size(); i++)
		{
			escapeUpgrades_[i]->DebugTree();
		}

		//ImGui::EndChild();
		ImGui::TreePop();
		ImGui::Separator();
	}
	ImGui::Separator();
	ImGui::Text("NowUpgrade : %d", upgradedConut_);
	if (ImGui::TreeNode("Selected"))
	{
		//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 70));

		for (size_t i = 0; i < attackUpgrades_.size(); i++)
		{
			if (attackUpgrades_[i]->isApplied)
			{
				attackUpgrades_[i]->DebugTree();
			}
		}
		//ImGui::EndChild();
		ImGui::Separator();
		//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 70));

		for (size_t i = 0; i < escapeUpgrades_.size(); i++)
		{
			if (escapeUpgrades_[i]->isApplied)
			{
				escapeUpgrades_[i]->DebugTree();
			}
		}

		//ImGui::EndChild();
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
	if (!isSelected_)
	{
		LWP::Info::SetDeltaTimeMultiply(0.0f);
		sprite_.isActive = true;
	}
	// 場所
	Vector2 pos{ 0.0f,625.0f };
	sprite_.isActive = true;
	pos.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2);
	// 抽選されたアップグレードを更新
	attackUpgrades_[candidata_[0]]->BaseUpdate();
	attackUpgrades_[candidata_[0]]->ShowUI(pos);
	// 
	pos.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2) * 2;
	// 抽選されたアップグレードを更新
	escapeUpgrades_[candidata_[1]]->BaseUpdate();
	escapeUpgrades_[candidata_[1]]->ShowUI(pos);

	// カーソルUI
	sprite_.transform.translation.x = LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2) * (cursorIndex_ + 1);


	lwp::Vector2 stick = LWP::Input::Controller::GetLStick();

	// 選択中は移動不可
	if (!isPress_ && !isSelected_)
	{
		// カーソル移動
		if (0 < cursorIndex_ &&
			(Input::Keyboard::GetTrigger(DIK_A) ||
				Input::Keyboard::GetTrigger(DIK_LEFT) ||
				Input::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT ||
					0.0f < stick.x)))
		{
			serectSE->Play();
			cursorIndex_--;
		}
		// 最後の添え字は決定
		if (cursorIndex_ < kUpgradNum_ &&
			(Input::Keyboard::GetTrigger(DIK_D) ||
				Input::Keyboard::GetTrigger(DIK_RIGHT) ||
				Input::Pad::GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT ||
					stick.x < 0.0f)))
		{
			serectSE->Play();
			cursorIndex_++;
		}
	}

	// 決定ボタンを押した
	if (Input::Keyboard::GetPress(DIK_SPACE) ||
		Input::Keyboard::GetPress(DIK_RETURN) ||
		Input::Pad::GetPress(XINPUT_GAMEPAD_A))
	{
		if (isPress_ == false)
		{
			SEvolume = 1.0f;
			chooseSE->Play(SEvolume);
		}
		isPress_ = true;

		CursorEffect_(10, sprite_.transform.translation);

		// カーソルをつぶしたり伸ばしたりする
		sprite_.transform.scale.y -= (sinf(pressTime_ * 60 * M_PI / 10) * 0.05f);
		sprite_.transform.scale.x += (sinf(pressTime_ * 60 * M_PI / 10) * 0.05f);
	}
	else
	{
		//だんだん音が下がる
		BGMt = (std::min)(BGMt + 0.01f, 1.0f);
		SEvolume = Lerp(SEvolume, 0.0f, BGMt);
		chooseSE->SetVolume(SEvolume);
		isPress_ = false;
		pressTime_ = 0.0f;
		sprite_.transform.scale = { 1,1,1 };
	}
	if (isPress_ && !isSelected_)
	{
		pressTime_ += lwp::GetDefaultDeltaTimeF();

		// 押している間はカーソルにモーションをつける
		if (selectMotion_.isEnd())
		{

		}

		if (kPressTime_ <= pressTime_)
		{
			choiceIndex_ = cursorIndex_;
			// 選択されたアップグレードを適応する
			if (kUpgradNum_ != 0)
			{
				Selected();
				selectedMotion_.Start();
			}
			isSelected_ = true;
			sprite_.isActive = false;
			pressTime_ = 0.0f;
		}
	}

	if (isSelected_)
	{
		Vector2 attackUpgradeAnimPos = { LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2), 625.0f };
		Vector2 escapeUpgradeAnimPos = { LWP::Info::GetWindowWidth() / float(kUpgradNum_ + 2) * 2, 625.0f };
		if (choiceIndex_ == 0)
		{
			attackUpgrades_[candidata_[0]]->ShowUI(attackUpgradeAnimPos + lwp::Vector2{ selectedAnimPos_.x, selectedAnimPos_.y });
			escapeUpgrades_[candidata_[1]]->ShowUI(lwp::Vector2{ -1000,0 });
		}
		else
		{
			escapeUpgrades_[candidata_[1]]->ShowUI(escapeUpgradeAnimPos + lwp::Vector2{ -selectedAnimPos_.x, selectedAnimPos_.y });
			attackUpgrades_[candidata_[0]]->ShowUI(lwp::Vector2{ -1000,0 });
		}
		// アップグレード選択時のアニメーション終了
		if (selectedMotion_.isEnd())
		{
			lwp::SetDeltaTimeMultiply(1.0f);
			// アップグレードが残ってなかったら適用のみ
			Apply(player);
			cursorIndex_ = 0;
			choiceIndex_ = 0;
			pressTime_ = 0.0f;
			selectedAnimPos_ = { 0,0 };
			isSelected_ = false;
		}
	}
	// カーソルのスプライトを上下に揺らす
	cursorAnimFrame_ += lwp::GetDefaultDeltaTimeF() * 60;
	sprite_.transform.translation.y += sinf(cursorAnimFrame_ * M_PI / 20) * 0.4f;
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

	// すべて適応
	for (size_t i = 0; i < attackUpgrades_.size(); i++)
	{
		// 描画を消す
		attackUpgrades_[i]->BaseUpdate();
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
		escapeUpgrades_[i]->BaseUpdate();
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
	// 敵を弾く
	player->StartEnemyKnockBack();
}

void L::UpgradeManager::CursorParticleInit()
{
	static LWP::Object::Particle CursorParticle_;
	CursorParticle_.SetPrimitive<Primitive::Sprite>();
	Primitive::Sprite particleTex;
	CursorParticle_.P()->texture = LWP::Resource::LoadTexture("CursorParticle.png");
	CursorParticle_.P()->isActive = true;
	CursorParticle_.P()->isUI = true;
	CursorParticle_.P()->material.shininess = 100.0f;
	CursorParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::GREEN);
	CursorParticle_.initFunction = [](Primitive::IPrimitive* primitive) {
		// 円周上に置く
		lwp::Vector3 pos = randomOnCircle();

		float dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		float dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		pos.x += dir1;
		pos.y += dir2;

		Object::ParticleData newData{};
		newData.wtf.translation = pos + primitive->transform.GetWorldPosition();
		newData.wtf.scale = { 0.1f,0.1f, 1.0f };

		// 中心までのベクトル
		newData.velocity = newData.wtf.translation;
		// パーティクル追加
		return newData;
		};
	CursorParticle_.updateFunction = [this](Object::ParticleData* data) {
		// 経過フレーム追加
		data->elapsedFrame++;
		lwp::Vector3 direction = sprite_.transform.translation - data->velocity;


		data->wtf.translation += direction / 10.0f;    // 速度ベクトルを加算

		return data->elapsedFrame > 10 ? true : false;
		};
	CursorParticle_.isActive = true;

	CursorEffect_ = [&](int i, lwp::Vector3 pos) {
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

lwp::Vector3 randomOnCircle()
{
	float ramdom = Utility::GenerateRandamNum<int>(0, 100);
	ramdom = ramdom / 100.0f;
	const float theta = 2.0 * std::numbers::pi * ramdom;
	return lwp::Vector3(std::cos(theta), std::sin(theta), 0.0f);
}

std::function<void(int, lwp::Vector3)> L::UpgradeManager::CursorEffect_ = nullptr;