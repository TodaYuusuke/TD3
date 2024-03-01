#pragma once

#include "Adapter.h"

#include <optional>

#include "Commands/PlayerInput.h"

class Player final
{
private:

	//*** Behaivior �ŊǗ����� ***//

	/// <summary>
	/// �v���C���[�s��
	/// </summary>
	enum class Behavior : size_t
	{
		Root = 0u,	// �ҋ@���
		Move,		// �ړ�
		//Attack,		// �����U��

		_COUNT,		// ��ԍő吔
	};

	//*** �e Behavior �Ŏg����� ***//

	struct IData
	{

	};

	struct RootData : public IData
	{

	};

	struct MoveData : public IData
	{

	};

public:

	Player() {}
	~Player();


	void Initialize();

	// �v���C���[�̑�����󂯕t����
	void UpdateInput();
	// �X�V
	void Update();

	lwp::WorldTransform* GetWorldTransform() { return &world_; }

public: //*** �R�}���h����ŌĂяo�����֐� ***//

	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();

private: //*** Behavior �Ǘ��Ɏg���֐� ***//

	void UpdateRoot();
	void UpdateMove();

private: //*** �v���C�x�[�g�ϐ� ***//

private: //*** �O������ݒ肷��ϐ� ***//

	float cPLAYERSPEED_ = 1.0f;

private: //*** �v�Z�Ɏg�� ***//

	// �v���C���[�̃C���v�b�g����������
	PlayerInput* pInput_ = nullptr;
	// �󂯎��R�}���h
	// �������͂ɑΉ���������
	IPlayerCommand* pCommand_ = nullptr;

	// �v���C���[�̃��f��
	LWP::Primitive::Mesh* demoModel;

	// ���[���h���W
	LWP::Object::WorldTransform world_;

	// ���݂̏��
	Behavior behavior_ = Behavior::Root;
	// ��Ԃ̗\��
	std::optional<Behavior> reqBehavior_ = std::nullopt;

	// ���Ɉړ����鑬�x
	LWP::Math::Vector3 destinate_;

};

