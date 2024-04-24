#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// 純粋な移動速度アップ！（定数）
	/// </summary>
	class Skill_BlowOffFlag : public ISkill
	{
	private:
	public:

		/// <summary>
		/// 純粋な移動速度アップ！（定数）
		/// <para>ここで値を定義する</para>
		/// <para>受け取った値によって読み込む画像を変えたい</para>
		/// </summary>
		/// <param name="v">プラスでもマイナスでもいい</param>
		Skill_BlowOffFlag() = default;



		/// <summary>
		/// アップグレードの内容
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->BlowOffFlag = true;
		}

		std::string GetTexturePass() override
		{
			return "powerUp/BlowOff.png";
		}

		std::string GetUpgradeName() override
		{
			return "BlowOffFlag";
		}
	};
}