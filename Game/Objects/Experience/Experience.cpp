#include "Experience.h"

using namespace LWP;
using namespace LWP::Resource;
using namespace LWP::Object::Collider;

//Experience* Experience::Create(const lwp::Vector3& pos)
//{
//	// 新規生成
//	Experience* data = new Experience;
//	// 初期化
//	data->Initialize(pos);
//	// 作ったものを返す
//	return data;
//}

Experience::Experience() : Experience({ 0.0f,0.0f,0.0f })
{
}

Experience::Experience(const lwp::Vector3& pos)
{
	Initialize(pos);
}

Experience::~Experience()
{
	// 表示も当たり判定も切る
	model_->isActive = false;
	collider_->isActive = false;
}

void Experience::Update()
{
}

void Experience::Initialize(const lwp::Vector3& pos)
{
	// モデル読み込み
	model_ = LoadModel("cube/cube.obj");
	// 設定
	model_->isActive = true;
	model_->name = "EXP";
	// 場所を設定
	model_->transform.translation = pos;

	// 当たり判定を作成
	CreateCollision();
	// 場所を設定
	collider_->Create(pos);

	// 死んでない
	isDead_ = false;

}

void Experience::CreateCollision()
{
	// 当たり判定を作成
	collider_ = CreateInstance<Sphere>();
	// マスク
	collider_->mask.SetBelongFrag(MaskLayer::Layer5);
	// プレイヤーと経験値取得範囲
	// プレイヤーとの当たり判定は消してもいい
	collider_->mask.SetHitFrag(MaskLayer::Layer4 | MaskLayer::Player);
	// 別個で用意した当たった時の関数
	// 状態を切り替えたい
	collider_->SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollision(data); });
	// 当たる
	collider_->isActive = true;
#ifdef DEMO
	collider_->name = "EXP";
#endif // DEMO
}

void Experience::OnCollision(const lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Trigger &&
		(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
	{
		// 本来はアニメーションをさせた後に消す
		isDead_ = true;
	}
}
