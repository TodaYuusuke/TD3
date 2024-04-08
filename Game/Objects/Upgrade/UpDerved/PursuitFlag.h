#pragma once
#pragma once
#include "../IUpgrade.h"

namespace L
{
	/// <summary>
	/// 純粋な移動速度アップ！（定数）
	/// </summary>
	class Skill_PursuitFlag : public ISkill
	{
	private:
		bool pursuitFlag_;
	public:
		Skill_PursuitFlag() {
			upgrades_.clear();
			upgrades_.push_back(new PowerDelta(5.0f));
		}
		/// <summary>
		/// 純粋な移動速度アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		Skill_PursuitFlag(bool flag = false) : pursuitFlag_(flag) {};



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->pursuitFlag = true;
		}

		std::string GetUpgradeName() override
		{
			return "PursuitFlag";
		}
	};
}