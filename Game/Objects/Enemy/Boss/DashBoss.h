#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"

class DashBoss : public IEnemy
{
public:// �p�u���b�N�ȃ����o�֐�
	//*** �������z�֐� ***//
	void Init()override;
	void Update()override;
	void SetPosition(lwp::Vector3 pos)override;

	///
	/// User Method
	/// 

	// �U������
	bool CheckAttackRange();

	// �Ώۂ�_��
	void Aim();

	/// Getter
	// ���@�Ƃ̕����x�N�g�����Z�o(�Ԃ�l�͐��K�����Ă���)
	LWP::Math::Vector3 GetDirectVel();

	/// Setter

private:// �v���C�x�[�g�ȃ����o�֐�
	//*** �������z�֐� ***//
	// �ړ�
	void Move()override;
	// �U��
	void Attack()override;

private:// �萔
	// �U������͈�
	const float kAttackRange = 10.0f;

	// �U���̃N�[���^�C��
	const int kAttackWaitTime = 120;

private:// �v���C�x�[�g�ȕϐ�
	// �_�b�V���̕����x�N�g��
	LWP::Math::Vector3 dashVel_;
};