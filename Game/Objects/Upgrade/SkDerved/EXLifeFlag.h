#pragma once
#pragma once
#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 純粋な移動速度アップ！（定数）
	/// </summary>
	class Skill_EXLifeFlag : public ISkill
	{
	private:
	public:

		/// <summary>
		/// 純粋な移動速度アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		Skill_EXLifeFlag() = default;



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->eXLifeFlag = true;
		}

		std::string GetTexturePass() override
		{
			return "Skill/EXLife.png";
		}

		std::string GetUpgradeName() override
		{
			return "EXLifeFlag";
		}
	};
}