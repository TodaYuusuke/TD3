#include "Level.h"

#include "Game/Objects/Player/Player.h"
#include "Game/Objects/GameMask.h"
#include "Game/Objects/Upgrade/UpgradeManager.h"

using namespace LWP;
using namespace LWP::Object::Collider;

Level::~Level()
{
}

void Level::Initialize(Player* p)
{
	player_ = p;
	prePos_ = player_->demoModel_.transform.translation;
	prePos_.y += 0.01f;
	// 経験値初期化
	exp_ = 0.0f;
	// とりあえず 10
	reqEXP_ = GetRequestEXP();

	bar_.reset(new ExpBar);
	bar_->Initialize();

	// 当たり判定生成
	CreateCollision();
	// 場所を設定
	//collider_->Create(position, position);
	collider_.Create(prePos_, player_->demoModel_.transform.translation, collider_.radius);
}

void Level::Update()
{
	// 当たり判定を 1 フレーム毎に更新
	//collider_->start = collider_->end;
	//collider_->end = position;
	collider_.Create(prePos_,
		player_->demoModel_.transform.translation,
		player_->parameter_.Other.radiusLevel);

	prePos_ = player_->demoModel_.transform.translation;
	prePos_.y += 0.01f;

	bar_->Update(reqEXP_, exp_);

#ifdef DEMO

	DebugWindow();

#endif // DEMO
}

void Level::CreateCollision()
{
	collider_.Create(prePos_, player_->demoModel_.transform.translation, collider_.radius);
	// マスク
	collider_.mask.SetBelongFrag(GameMask::ExpGetter());
	// 経験値
	collider_.mask.SetHitFrag(GameMask::Exp());
	// 別個で用意した当たった時の関数
	// 状態を切り替えたい
	collider_.SetOnCollisionLambda([this](HitData data) {OnCollision(data); });
	// 当たる
	collider_.isActive = true;
#ifdef DEMO
	collider_.name = "Level";
#endif // DEMO
	collider_.radius = player_->config_.Other_.RADIUSLEVEL_;
}

void Level::OnCollision(const lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Trigger &&
		(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
	{
		// 経験値取得
		GainEXP();
	}
}

void Level::GainEXP()
{
	// 既に上限を迎えていたら処理しない
	/*if (reqEXP_ <= exp_)
	{
		return;
	}*/
	// 経験値補正入れるならここ
	exp_ += 1.0f;
	// レベルアップ
	if (reqEXP_ <= exp_)
	{
		LevelUp();
	}
}

void Level::LevelUp()
{
	// プレイヤーの HP を回復させたい
	player_->IncreaseHP();
	bar_->Update(reqEXP_, exp_);
	// 敵を弾く
	player_->StartEnemyKnockBack();
	exp_ = 0.0f;
	if (L::UpgradeManager::GetMaxLevel() <= lv_)
	{
		return;
	}
	reqEXP_ = GetRequestEXP();
	lv_++;
	// ここでアップデートする関数を呼び出す
	L::UpgradeManager::LevelUp();
}

#ifdef DEMO
void Level::DebugWindow()
{
	ImGui::Begin("Levels");

	//if (ImGui::BeginTabBar("exp"))
	//{

	//	if (ImGui::BeginTabItem("Level"))
	//	{

	if (ImGui::TreeNode("Level"))
	{
		ImGui::Text("lv      : %d", lv_);
		ImGui::Text("Gain    : Button or Press 9");
		ImGui::Text("LevelUp : Button or Press 0");

		if (ImGui::Button("Gain") || lwp::Keyboard::GetTrigger(DIK_9))
		{
			GainEXP();
		}

		if (ImGui::Button("LevelUp") || lwp::Keyboard::GetTrigger(DIK_0))
		{
			LevelUp();
		}

		ImGui::Separator();

		ImGui::Text("LEVEL:%d", lv_);
		ImGui::DragFloat("REQEXP", &reqEXP_, 1.0f);
		ImGui::DragFloat("EXP", &exp_, 1.0f);
		ImGui::Separator();

		ImGui::TreePop();
	}
	//		ImGui::EndTabItem();
	//	}

	//	ImGui::EndTabBar();
	//}

	ImGui::End();
}
#endif // DEMO


