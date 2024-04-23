#pragma once
#pragma once
#pragma once
#include "../ISkill.h"

namespace L
{
	/// <summary>
	/// �����Ȉړ����x�A�b�v�I�i�萔�j
	/// </summary>
	class Skill_EXLifeFlag : public ISkill
	{
	private:
	public:

		/// <summary>
		/// �����Ȉړ����x�A�b�v�I�i�萔�j
		/// <para>�����Œl���`����</para>
		/// <para>�󂯎�����l�ɂ���ēǂݍ��މ摜��ς�����</para>
		/// </summary>
		/// <param name="v">�v���X�ł��}�C�i�X�ł�����</param>
		Skill_EXLifeFlag() = default;



		/// <summary>
		/// �A�b�v�O���[�h�̓��e
		/// </summary>
		void Apply(UpgradeParameter* para) override
		{
			para->eXLifeFlag = true;
		}

		std::string GetTexturePass() override
		{
			return "powerUp/Pursuit.png";
		}

		std::string GetUpgradeName() override
		{
			return "EXLifeFlag";
		}
	};
}