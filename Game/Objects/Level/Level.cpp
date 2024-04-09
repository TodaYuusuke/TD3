#include "Level.h"
#include "Game/Objects/Upgrade/UpgradeManager.h"

using namespace LWP;
using namespace LWP::Object::Collider;

Level::~Level()
{
}

void Level::Initialize(const lwp::Vector3& position)
{
	// 経験値初期化
	exp_ = 0.0f;
	// とりあえず 10
	reqEXP_ = 10.0f;

	// 当たり判定生成
	CreateCollision();
	// 場所を設定
	//collider_->Create(position, position);
	collider_.Create(position);
}

void Level::Update(const lwp::Vector3& position)
{
	// 当たり判定を 1 フレーム毎に更新
	//collider_->start = collider_->end;
	//collider_->end = position;
	collider_.Create(position);


#ifdef DEMO

	DebugWindow();

#endif // DEMO
}

void Level::CreateCollision()
{
	// マスク
	collider_.mask.SetBelongFrag(MaskLayer::Layer4);
	// 経験値
	collider_.mask.SetHitFrag(MaskLayer::Layer5);
	// 別個で用意した当たった時の関数
	// 状態を切り替えたい
	collider_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollision(data); });
	// 当たる
	collider_.isActive = true;
	collider_.name = "Level";
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
	exp_ = 0.0f;
	reqEXP_ += 1.0f;
	lv_++;
	// ここでアップデートする関数を呼び出す
	L::UpgradeManager::LevelUp();
}

#if DEMO

void Level::DebugWindow()
{
	ImGui::Begin("Levels");

	//if (ImGui::BeginTabBar("exp"))
	//{

	//	if (ImGui::BeginTabItem("Level"))
	//	{

	if (ImGui::TreeNode("Level"))
	{
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

#endif