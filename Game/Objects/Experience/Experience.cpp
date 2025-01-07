#include "Experience.h"

#include "Game/Objects/GameMask.h"

using namespace LWP;
using namespace LWP::Resource;
using namespace LWP::Object::Collider;


std::function<void(int, lwp::Vector3)> Experience::expGetEffect_;

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
	// モデルの回転
	model_.transform.rotation.y += rotateSpeed_;
	if (model_.transform.rotation.y < -3.14f)
	{
		model_.transform.rotation.y += 3.14f;
	}
	else if (3.14f < model_.transform.rotation.y)
	{
		model_.transform.rotation.y -= 3.14f;
	}
	// 発生してから数秒は判定を付与しない
	if (isDisable_)
	{
		if (kToEnableTime_ < time_)
		{
			time_ = 0.0f;
			collider_.isActive = true;
			isDisable_ = false;
			return;
		}
		// 時間を加算
		time_ += lwp::GetDefaultDeltaTimeF();
	}
	// アニメーションが開始していたら
	else if (lvPosition_)
	{
		if (kAnimationTime_ <= time_)
		{
			isDead_ = true;
			//expGetEffect_(16, model_.transform.translation);
			model_.isActive = false;
			return;
		}
		// 位置を参照
		lwp::Vector3 move = *lvPosition_ - collider_.position;
		float t = std::clamp(time_ / kAnimationTime_, 0.0f, 1.0f);
		// タメがあるような移動
		model_.transform.translation = collider_.position + Utility::Easing::InBack(t) * move;
		// 1.0f ~ 0.0f に再計算
		t = (1.0f - LWP::Utility::Easing::InExpo(t));
		// 一定以下なら 0.0f にする(描画しない)
		t = 0.2 < t ? t : 0.0f;
		// このイージングでないとよく見えなかった
		model_.transform.scale = lwp::Vector3(kSize_, kSize_, kSize_) * t;
		// 時間を加算
		time_ += lwp::GetDefaultDeltaTimeF();
	}
}

void Experience::InitParticle()
{
	static Object::Particle expGetParticle;
	expGetParticle.SetPrimitive<Primitive::Cube>();
	expGetParticle.P()->material.enableLighting = true;
	expGetParticle.name = "EXPParticle";
	//expGetParticle.P()->transform.scale = { 0.0001f,0.0001f, 0.0001f };
	expGetParticle.P()->commonColor = new Utility::Color(0xAAAA55FF);
	expGetParticle.initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};

		newData.wtf = primitive->transform;
		newData.wtf.scale = { 0.3f,0.3f,0.3f };

		// 方向ベクトルを生成
		int dirX = Utility::GenerateRandamNum<int>(-200, 200);
		int dirY = Utility::GenerateRandamNum<int>(-200, 200);
		int dirZ = Utility::GenerateRandamNum<int>(-200, 200);
		// 速度ベクトル
		Math::Vector3 dir{ dirX * 0.01f, dirY * 0.01f, dirZ * 0.01f };
		float multiply = Utility::GenerateRandamNum<int>(10, 30) * 0.05f;
		newData.velocity = dir.Normalize() * multiply;

		//for (size_t i = 0; i < primitive->vertices.size(); i++)
		//{
		//	primitive[i].transform.translation.z = -10.0f;
		//}

		// パーティクル追加
		return newData;
		};
	// パーティクルの更新処理
	expGetParticle.updateFunction = [](Object::ParticleData* data) {
		if (Info::GetDeltaTimeF() == 0.0f)
		{
			return false;
		}

		// 経過秒数追加
		data->elapsedFrame++;

		// 速度ベクトルを加算
		data->wtf.translation += data->velocity;

		// 奥方向に加算
		//data->wtf.translation.z += -9.8f / 80.0f;

		// 少しづつ小さくする
		data->wtf.scale *= 0.92f;
		data->velocity.y += 0.07f;
		// 速度ベクトルを弱める
		data->velocity *= 0.9f;

		// 一定時間経ったら消す
		return 20 <= data->elapsedFrame ? true : false;
		};
	expGetParticle.isActive = true;
	expGetEffect_ = [&](int i, lwp::Vector3 pos) {
		expGetParticle.P()->transform = pos;
		expGetParticle.Add(i);
		};
}

void Experience::Initialize(const lwp::Vector3& pos)
{
	// モデル読み込み
	model_.LoadFile("Exp/Exp.obj");
	// 設定
	model_.isActive = true;
	model_.name = "EXP";
	// 場所を設定
	model_.transform.translation = pos;
	model_.transform.scale = lwp::Vector3(kSize_, kSize_, kSize_);
	// 色
	model_.commonColor = new Utility::Color(0x7CFC00FF);

	model_.material.enableLighting = true;

	// 場所を設定
	collider_.Create(pos);
	// 当たり判定を作成
	CreateCollision();

	// 死んでない
	isDead_ = false;
	isDisable_ = true;

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
		// 取得時に回転スピードを上げる
		rotateSpeed_ = kRotateGetSpeed_;
		expGetEffect_(16, model_.transform.translation);
	}
}
