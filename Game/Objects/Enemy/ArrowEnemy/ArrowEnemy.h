#pragma once
#include "Game/Objects/Enemy/IEnemy/IEnemy.h"
#include "Arrow/Arrow.h"

class ArrowEnemy : public IEnemy
{
public:// �p�u���b�N�ȃ����o�֐�
	/// 
	/// Default Method
	/// 

	void Init()override;
	void Update()override;

	///
	/// User Method
	/// 

	/// Getter
	// �_���Ώۂ��擾
	const LWP::Math::Vector3& GetAimTarget() { return *target_; }

	/// Setter
	// �_���Ώۂ�ݒ�
	void SetAimTarget(LWP::Math::Vector3* target) { target_ = target; }

private:// �v���C�x�[�g�ȃ����o�֐�
	// �ړ�
	void Move(LWP::Math::Vector3 MoveVec)override;
	// �U��
	void Attack()override;
	// �Ώۂ�_��
	void Aim();

private:// �����o�ϐ�
	std::list<std::unique_ptr<Arrow>> arrows_;
	// �_���Ώ�
	LWP::Math::Vector3* target_;
};