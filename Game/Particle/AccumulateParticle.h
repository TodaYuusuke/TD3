#pragma once
#include <Adapter.h>

/// <summary>
/// パーティクル
/// </summary>
class AccumulateParticle : public LWP::Object::Particle<LWP::Resource::Cube> {
private: // ** 純粋仮想関数のオーバーライド ** //

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override {
		newData.m.worldTF.translation = lwp::Vector3{ 0,-0.5f,0 } + model.worldTF.GetWorldPosition();
		newData.m.worldTF.scale = { 0.5f,0.5f, 0.5f };

		// 速度ベクトルを生成
		int dir1 = LWP::Utility::GenerateRandamNum<int>(-10, 10);
		int dir2 = LWP::Utility::GenerateRandamNum<int>(-1, 5);
		int dir3 = LWP::Utility::GenerateRandamNum<int>(-10, 10);
		// 発射のベクトル
		LWP::Math::Vector3 dir{ dir1 / 100.0f,dir2 / 200.0f, dir3 / 100.0f };
		float multiply = LWP::Utility::GenerateRandamNum<int>(10, 50) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;
	};
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(Data& data) override {
		if (LWP::Info::GetDeltaTime() == 0.0f) { return false; }

		// 経過フレーム追加
		data.elapsedFrame++;

		// 速度ベクトルを弱める
		data.velocity.x *= 0.9f;
		data.velocity.z *= 0.9f;

		// 重力を加算
		data.velocity.y += 9.8f / 2000.0f;

		// 速度ベクトルを加算
		data.m.worldTF.translation += data.velocity;
		data.m.worldTF.rotation += data.velocity;

		return data.elapsedFrame > 120 ? true : false;
	};
};
