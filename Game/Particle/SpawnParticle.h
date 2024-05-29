#pragma once
#include <Adapter.h>

/// <summary>
/// パーティクル
/// </summary>
class SpawnParticle : public LWP::Object::Particle<LWP::Resource::Cube> {
private: // ** 純粋仮想関数のオーバーライド ** //

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override {
		newData.m.worldTF.translation = model.worldTF.GetWorldPosition();
		// 大きさをランダムにする
		int scale = LWP::Utility::GenerateRandamNum<int>(25, 40);
		newData.m.worldTF.scale = { scale / 130.0f, scale / 130.0f, scale / 130.0f };

		// 速度ベクトルを生成
		int dir1 = LWP::Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = LWP::Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = LWP::Utility::GenerateRandamNum<int>(-100, 100);
		// 発射のベクトル
		LWP::Math::Vector3 dir{ dir1 / 100.0f,dir2 / 150.0f, dir3 / 100.0f };
		// 係数
		float multiply = LWP::Utility::GenerateRandamNum<int>(20, 50) / 100.0f;
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

		data.m.worldTF.translation += data.velocity;    // 速度ベクトルを加算
		data.m.worldTF.rotation += data.velocity;    // ついでに回転させとく


		// 20フレーム以降から重力を加算
		if (data.elapsedFrame > 10)
		{
			data.velocity.y += -9.8f / 800.0f;
			// yが0以下になったとき跳ねる
			if (data.m.worldTF.translation.y <= 0.1f)
			{
				data.velocity.y *= -0.5f;
			}
		}
		else
		{
			// 速度ベクトルを弱める
			data.velocity *= 0.9f;
		}

		// ちょっとしたら検証開始

		// 速度が極端に遅くなったら終了フェーズ
		if (data.elapsedFrame > 15 &&
			data.velocity.y <= 0.01f && -0.01f <= data.velocity.y &&
			data.m.worldTF.translation.y <= 0.15f && data.m.worldTF.translation.y >= -0.15f)
		{
			data.velocity = { 0.0f,0.0f,0.0f };
			data.m.worldTF.scale *= 0.9f;
			// もし完全に小さくなったなら終了
			if (data.m.worldTF.scale.x <= 0.001f) { return true; }
		}

		return false;
	};
};
