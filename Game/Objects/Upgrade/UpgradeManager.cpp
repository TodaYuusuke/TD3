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

	// 選択中のパーティクル
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
	LWP::Info::SetDeltaTimeMultiply(0.0f);
	// 場所
	Vector2 pos{ 0.0f,625.0f };
	sprite_.isActive = true;
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
		// サイズ　1920 x 1080
		lwp::Matrix4x4 matViewport = MakeViewportMatrix(0, 0,1980, 1080, 0, 1);
		lwp::Matrix4x4 viewProj = mainCameraptr_->GetViewProjection();
		lwp::Matrix4x4 matVPV = viewProj * matViewport;
		matVPV = matVPV.Inverse();

		lwp::Vector3 posNear = Vector3(sprite_.transform.translation.x, sprite_.transform.translation.y, 0);
		lwp::Vector3 posFar = Vector3(sprite_.transform.translation.x, sprite_.transform.translation.y, 1);

		posNear = lwp::Matrix4x4::TransformCoord(posNear,matVPV);
		posFar = lwp::Matrix4x4::TransformCoord(posFar,matVPV);
		centerPoint = posFar - posNear;
		centerPoint = centerPoint.Normalize();
		centerPoint = posNear + centerPoint * 50;
		if (cursorIndex_ == 0) {
			centerPoint.x += 0.5f;
		}
		else if (cursorIndex_ == 1) {
			centerPoint.x += 0.8f;
		}
		CursorEffect_(2, centerPoint);

		sprite_.transform.scale.y -= (sinf(pressTime_ * 60 * M_PI / 10) * 0.05f);
		sprite_.transform.scale.x += (sinf(pressTime_ * 60 * M_PI / 10) * 0.05f);
	}
	else
	{
		isPress_ = false;
		pressTime_ = 0.0f;
		sprite_.transform.scale = { 1,1,1 };
	}
	if (isPress_)
	{
		pressTime_ += lwp::GetDefaultDeltaTimeF();

		// 押している間はカーソルにモーションをつける
		if (selectMotion_.isEnd()) {
			
		}

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
	LWP::Info::SetDeltaTimeMultiply(1.0f);
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
	CursorParticle_.P()->transform.scale = { 0.0001f,0.0001f, 0.0001f };
	CursorParticle_.P()->material.shininess = 100.0f;
	CursorParticle_.P()->commonColor = new Utility::Color(Utility::ColorPattern::GREEN);
	CursorParticle_.initFunction = [](Primitive::IPrimitive* primitive)
		{
			// 円周上に置く
			lwp::Vector3 pos = randomOnCircle();
			pos *= 1.5f;
			float dir1 = Utility::GenerateRandamNum<int>(-50, 50);
			dir1 = dir1 / 100.0f;
			float dir2 = Utility::GenerateRandamNum<int>(-50, 50);
			dir2 = dir2 / 100.0f;
			pos.x += dir1;
			pos.y += dir2;

			Object::ParticleData newData{};
			newData.wtf.translation = pos + primitive->transform.GetWorldPosition();
			newData.wtf.rotation = primitive->transform.rotation;
			newData.wtf.scale = { 0.25f,0.25f, 0.25f };
			//lwp::Vector3 
			// 中心までのベクトル
			newData.velocity = newData.wtf.translation;
			// パーティクル追加
			return newData;
		};
	CursorParticle_.updateFunction = [this](Object::ParticleData* data)
		{
			// 経過フレーム追加
			data->elapsedFrame++;
			lwp::Vector3 direction = data->velocity - centerPoint;


			data->wtf.translation -= direction / 10.0f;    // 速度ベクトルを加算
			data->wtf.rotation += data->velocity;    // ついでに回転させとく

			return data->elapsedFrame > 10 ? true : false;
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

lwp::Vector3 randomOnCircle() {
	float ramdom = Utility::GenerateRandamNum<int>(0, 100);
	ramdom = ramdom / 100.0f;
	const float theta = 2.0 * std::numbers::pi * ramdom;
	return lwp::Vector3(std::cos(theta), std::sin(theta), 0.0f);
}

std::function<void(int, lwp::Vector3)> L::UpgradeManager::CursorEffect_ = nullptr;