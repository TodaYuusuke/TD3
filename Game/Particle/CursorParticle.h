#pragma once
#include <Adapter.h>

/// <summary>
/// パーティクル
/// </summary>
class CursorParticle : public LWP::Object::Particle<LWP::Resource::Cube> {
private: // ** 純粋仮想関数のオーバーライド ** //

	LWP::Math::Vector3 centerPoint;

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override {
		// 円周上に置く
		lwp::Vector3 pos = randomOnCircle();
		pos *= 1.5f;
		float dir1 = LWP::Utility::GenerateRandamNum<int>(-50, 50);
		dir1 = dir1 / 100.0f;
		float dir2 = LWP::Utility::GenerateRandamNum<int>(-50, 50);
		dir2 = dir2 / 100.0f;
		pos.x += dir1;
		pos.y += dir2;

		newData.m.worldTF.translation = pos + model.worldTF.GetWorldPosition();
		newData.m.worldTF.scale = { 0.25f,0.25f, 0.25f };

		// 中心までのベクトル
		newData.velocity = newData.m.worldTF.translation;
	};
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(Data& data) override {
		// 経過フレーム追加
		data.elapsedFrame++;
		lwp::Vector3 direction = data.velocity - centerPoint;


		data.m.worldTF.translation -= direction / 10.0f;    // 速度ベクトルを加算
		data.m.worldTF.rotation += data.velocity;    // ついでに回転させとく

		return data.elapsedFrame > 10 ? true : false;
	};
};
