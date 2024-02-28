#pragma once
#include <scene/IScene.h>
#include "Game/Objects/FollowCamera/FollowCamera.h"

class TItleScene final
	: public IScene
{
public:
	TItleScene() = default;	// �R���X�g���N�^�͎��R�ɒ�`�\
	~TItleScene() = default;

	//*** �������z�֐��̎��̐錾 ***//

	// ������
	void Initialize() override;
	// �X�V
	void Update() override;

	// scene�J�ڂ������ꍇ�AIScene�|�C���^�^��nextScene�Ɏ��̃V�[���̎��̂���

private: //*** �������ɕK�v�ȏ�����ϐ����L�q ***//

	LWP::Primitive::Triangle* tri[2];
	LWP::Primitive::Surface* surface;
	LWP::Primitive::Sphere* sphere;

	LWP::Primitive::Mesh* cubeModel;
	LWP::Primitive::Mesh* stressTestModel;	// �`�敉�׌��ؗp���f��

	LWP::Resource::Texture* uvTexture;
	LWP::Resource::Texture* monsterBall;

	// SE�f�[�^
	LWP::Resource::Audio* audio;

	// �T�u�J����
	LWP::Object::Camera* subCamera;
	// �Ǐ]�J����
	std::unique_ptr<FollowCamera> followCamera_;

	// �p�[�e�B�N��
	LWP::Object::Particle* particle;

	bool useMonsterBall = true;
};