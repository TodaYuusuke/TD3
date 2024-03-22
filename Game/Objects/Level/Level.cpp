#include "Level.h"

using namespace LWP;
using namespace LWP::Object::Collider;

Level::~Level()
{
}

void Level::Initialize()
{
	// 経験値初期化
	exp_ = 0.0f;
	// とりあえず 10
	reqEXP_ = 10.0f;

	// 当たり判定生成
	CreateCollision();
}

void Level::Update(const lwp::Vector3& position)
{
	// 当たり判定を 1 フレーム毎に更新
	collider_->start = collider_->end;
	collider_->end = position;



#ifdef DEMO

	//DebugWindow();

#endif // DEMO
}

void Level::CreateCollision()
{
	// 当たり判定を作成
	collider_ = CreateInstance<Capsule>();
	// マスク
	collider_->mask.SetBelongFrag(MaskLayer::Layer4);
	// 経験値
	collider_->mask.SetHitFrag(MaskLayer::Layer5);
	// 別個で用意した当たった時の関数
	// 状態を切り替えたい
	collider_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollision(data); });
	// 当たる
	collider_->isActive = true;
}

void Level::OnCollision(const lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Trigger &&
		data.hit->mask.GetBelongFrag() & MaskLayer::Layer5)
	{
		// 経験値取得
		GainEXP();
		// レベルアップ
		if (reqEXP_ <= exp_)
		{
			LevelUp();
		}
	}
}

void Level::GainEXP()
{
	// 経験値補正入れるならここ
	exp_ += 1.0f;
}

void Level::LevelUp()
{
	exp_ = 0.0f;
	reqEXP_ += 1.0f;
	// ここでアップデートする関数を呼び出す

}

void Level::DebugWindow()
{
	ImGui::Begin("Level");



	ImGui::End();
}
