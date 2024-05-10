#include "Experience.h"

#include "Game/Objects/GameMask.h"

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

Experience::~Experience() {}

void Experience::Update()
{
	// アニメーションが開始していたら
	if (lvPosition_ && !collider_.isActive)
	{
		if (animationTime_ <= time_)
		{
			isDead_ = true;
			return;
		}
		// 位置を参照
		lwp::Vector3 move = *lvPosition_ - model_.transform.translation;
		float t = time_ / animationTime_;
		// タメがあるような移動
		model_.transform.translation += Utility::Easing::InBack(t) * move;
		// このイージングでないとよく見えなかった
		model_.transform.scale = lwp::Vector3(1.0f, 1.0f, 1.0f) * (1.0f - LWP::Utility::Easing::InExpo(t));
	}
	// 発生してから数秒は判定を付与しない
	else
	{
		if (toEnableTime_ < time_)
		{
			time_ = 0.0f;
			collider_.isActive = true;
			return;
		}
	}
	// 時間を加算
	time_ += lwp::GetDefaultDeltaTimeF();
	//time_ += 0.005f;
}

void Experience::Initialize(const lwp::Vector3& pos)
{
	// モデル読み込み
	model_.LoadFile("cube/cube.obj");
	// 設定
	model_.isActive = true;
	model_.name = "EXP";
	// 場所を設定
	model_.transform.translation = pos;

	// 場所を設定
	collider_.Create(pos);
	// 当たり判定を作成
	CreateCollision();

	// 死んでない
	isDead_ = false;


}

void Experience::CreateCollision()
{
	// マスク
	collider_.mask.SetBelongFrag(GameMask::Exp());
	// プレイヤーと経験値取得範囲
	// プレイヤーとの当たり判定は消してもいい
	collider_.mask.SetHitFrag(GameMask::ExpGetter());
	// 別個で用意した当たった時の関数
	// 状態を切り替えたい
	collider_.SetOnCollisionLambda([this](HitData data) {OnCollision(data); });
	collider_.name = "EXP";
	// 一定時間経つまで取得不可
	collider_.isActive = false;
	// 判定を描画しない
#ifdef DEMO
	collider_.isShowWireFrame = false;
#endif 
}

void Experience::OnCollision(const lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Press &&
		(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag()))
	{
		// プレイヤーの位置を参照し続ける
		Capsule* lv = dynamic_cast<Capsule*>(data.hit);
		assert(lv);
		lvPosition_ = &lv->end;

		//LWP::Utility::Easing::InOutExpo,LWP::Utility::Easing::InOutExpo);
		// 本来はアニメーションをさせた後に消す
		//isDead_ = true;
		collider_.isActive = false;
	}
}
