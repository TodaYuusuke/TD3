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
	public:

		/// <summary>
		/// 純粋な移動速度アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		Skill_PursuitFlag() = default;



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->pursuitFlag = true;
		}

		std::string GetTexturePass() override
		{
			return "powerUp/Pursuit.png";
		}

		std::string GetUpgradeName() override
		{
			return "PursuitFlag";
		}
	};
}