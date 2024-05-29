#pragma once
#include <Adapter.h>

/// <summary>
/// パーティクル
/// </summary>
class MissileContrailParticle : public LWP::Object::Particle<LWP::Resource::Cube> {
private: // ** 純粋仮想関数のオーバーライド ** //

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override {
		newData.m.worldTF.translation = model.worldTF.GetWorldPosition();
		// 大きさをランダムにする
		float scale = LWP::Utility::GenerateRandamNum<int>(45, 50);
		newData.m.worldTF.rotation = { scale, scale, scale };
		newData.m.worldTF.scale = { scale / 100.0f, scale / 100.0f, scale / 100.0f };
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

		data.m.worldTF.translation.y += 0.0001f * data.elapsedFrame;
		//data->wtf.scale *= 0.92f;
		float f = 0.05f;
		data.m.worldTF.scale -= { f, f, f };

		// 速度は徐々に落とす
		data.velocity *= 0.9f;

		// 180フレーム経ったら削除
		if (data.m.worldTF.scale.x <= 0.001f) { return true; }
		return data.elapsedFrame > 240 ? true : false;
	};
};
