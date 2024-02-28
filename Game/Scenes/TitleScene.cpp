#include "TitleScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// ������
void TItleScene::Initialize()
{
	// �f�o�b�O���\��
	Info::ChangeShowDebugGUI();

	// �e�N�X�`���ǂݍ���
	uvTexture = LWP::Resource::LoadTextureLongPath("resources/system/texture/uvChecker.png");
	monsterBall = LWP::Resource::LoadTextureLongPath("resources/system/texture/monsterBall.png");

	// �T�u�J�����𐶐�
	subCamera = LWP::Object::CreateInstance<LWP::Object::Camera>();
	LWP::Object::CreateRenderTexture(subCamera, 640, 360);
	subCamera->transform.translation = { 5.9f,4.5f,-10.0f };
	subCamera->transform.rotation = { 0.4f,-0.5f,0.0f };
	// �|�X�g�v���Z�X�e�X�g
	subCamera->isUsePostProcess = true;
	subCamera->isActive = true;

	// �T�u�J�����𐶐�
	LWP::Object::Camera* c = LWP::Object::CreateInstance<LWP::Object::Camera>();
	c->shaderPath = "postProcess/SSAO.PS.hlsl";
	LWP::Object::CreateRenderTexture(c, 640, 360);
	c->transform.translation = { -5.9f,4.5f,-10.0f };
	c->transform.rotation = { 0.4f,0.5f,0.0f };
	// �|�X�g�v���Z�X�e�X�g
	c->isUsePostProcess = true;
	c->isActive = true;

	//mainCamera->isUsePostProcess = true;
	mainCamera->shaderPath = "postProcess/SSAO.PS.hlsl";
	mainCamera->ReCreateShader();
	//SetMainRenderCamera(subCamera);

	// �n��
	Mesh* ground = LWP::Resource::LoadModel("cube/cube.obj");
	ground->transform.translation.y = -1.5f;
	ground->transform.scale = { 10.0f,0.1f, 10.0f };
	//ground->material.enableLighting = true;
	ground->name = "Ground";
	//ground->commonColor = new Color()

	// �O�p�`
	for (int i = 0; i < 2; i++)
	{
		tri[i] = LWP::Primitive::CreateInstance<Triangle>();
	}
	tri[0]->vertices[0].color = Color(RED);
	tri[0]->vertices[1].color = Color(BLUE);
	tri[0]->vertices[2].color = Color(GREEN);
	tri[1]->transform.rotation.y = 1.0f;
	tri[1]->texture = uvTexture;

	// ����
	surface = LWP::Primitive::CreateInstance<Surface>();
	surface->transform.translation.x = -0.7f;
	surface->texture = uvTexture;
	surface->isActive = false;

	// ��
	sphere = LWP::Primitive::CreateInstance<Sphere>();
	sphere->Radius(0.3f);
	sphere->transform.translation.x = -1.0f;
	sphere->transform.translation.z = -1.0f;
	//sphere->material.enableLighting = true;
	sphere->material.shininess = 40.0f;
	sphere->texture = uvTexture;

	// ���f���ǂݍ���
	cubeModel = LWP::Resource::LoadModel("cube/cube.obj");
	cubeModel->transform.translation.y = -3.0f;
	cubeModel->transform.scale = { 5.0f,5.0f, 0.05f };
	cubeModel->isActive = true;
	//cubeModel->material.enableLighting = true;
	//stressTestModel = LWP::Resource::LoadModel("RGM-96XJesta_13_td.obj");
	//stressTestModel->material.enableLighting = true;
	//stressTestModel->isActive = true;
	// �����ɕǂ�u��
	Primitive::Mesh* cube2 = LWP::Resource::LoadModel("cube/cube.obj");
	cube2->transform.translation = { -2.1f, -3.0f, -2.0f };
	cube2->transform.rotation.y = -1.54f;
	cube2->transform.scale = { 5.0f,5.0f, 0.05f };
	cube2->isActive = true;


	// ���s����
	Object::DirectionLight* dirLight = Object::CreateInstance<Object::DirectionLight>();
	dirLight->isActive = true;
	dirLight->intensity = 0.3f;

	// �_����
	Object::PointLight* pL1 = Object::CreateInstance<Object::PointLight>();
	pL1->transform.translation = { 1.6f,0.0f,-0.1f };
	pL1->intensity = 0.3f;
	pL1->isActive = true;
	Object::PointLight* pL2 = Object::CreateInstance<Object::PointLight>();
	pL2->transform.translation = { -1.5f,-0.1f,-0.1f };
	pL2->intensity = 0.3f;
	pL2->isActive = true;

	// ������ʕ`��̌��ʂ𒣂�t����X�v���C�g
	Sprite* s = LWP::Primitive::CreateInstance<Sprite>();
	s->texture = subCamera->GetRenderTexture();
	s->isUI = true;
	Sprite* s2 = LWP::Primitive::CreateInstance<Sprite>();
	s2->transform.translation.x = 640.0f;
	s2->texture = c->GetRenderTexture();
	s2->isUI = true;


	// �p�[�e�B�N���e�X�g
	particle = Object::CreateInstance<Object::Particle>();
	particle->primitive = LWP::Resource::LoadModel("cube/cube.obj");
	particle->initFunction = [](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		newData.wtf.scale = primitive->transform.scale;

		// ���x�x�N�g���𐶐�
		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// ���˂̃x�N�g��
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		newData.velocity = dir.Normalize() * 0.2f;

		// �p�[�e�B�N���ǉ�
		return newData;
		};
	particle->updateFunction = [](Object::ParticleData* data) {
		// �o�߃t���[���ǉ�
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;	// ���x�x�N�g�������Z
		data->wtf.rotation += data->velocity;	// ���łɉ�]�����Ƃ�
		data->wtf.translation.y += -9.8f / 100.0f;	// �d�͂����Z

		// ���x�x�N�g������߂�
		data->velocity *= 0.9f;

		return data->elapsedFrame > 180 ? true : false;
		};
	particle->isActive = true;

	// �Ǐ]�J����
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&sphere->transform);
}

// �X�V
void TItleScene::Update()
{
	// �Ǐ]�J����
	followCamera_->Update();
	mainCamera->transform = followCamera_->viewProjection_.transform;

	// �|�X�g�v���Z�X�̐؂�ւ�
	if (Keyboard::GetTrigger(DIK_SPACE))
	{
		mainCamera->isUsePostProcess = !mainCamera->isUsePostProcess;
	}

	// T�L�[�������ƃe�N�X�`���؂�ւ�
	if (Keyboard::GetTrigger(DIK_T))
	{
		if (!useMonsterBall)
		{
			sphere->texture = monsterBall;
			useMonsterBall = true;
		}
		else
		{
			sphere->texture = uvTexture;
			useMonsterBall = false;
		}
	}

	if (Keyboard::GetTrigger(DIK_1))
	{
		LWP::Window::ChangeWindowMode();
	}
	else if (Keyboard::GetTrigger(DIK_2))
	{
		LWP::Window::ChangeFullScreenMode();
	}
	else if (Keyboard::GetTrigger(DIK_3))
	{
		LWP::Window::ChangeBorderlessWindowMode();
	}

	// �V�F�[�_�[��蒼��
	if (Keyboard::GetTrigger(DIK_R))
	{
		mainCamera->ReCreateShader();
	}
	// �p�[�e�B�N���Ăяo��
	if (Keyboard::GetTrigger(DIK_P))
	{
		particle->Add(16);
	}
	// �v���O�����I��
	if (Keyboard::GetTrigger(DIK_O))
	{
		LWP::System::End();
	}

	// ESC�L�[�Ńf�o�b�O���\���؂�ւ�
	if (Keyboard::GetTrigger(DIK_ESCAPE))
	{
		Info::ChangeShowDebugGUI();
	}

	// ENTER�L�[�������ƃV�[���؂�ւ�
	if (Keyboard::GetTrigger(DIK_N))
	{
		//nextScene_ = new SampleGameScene();
	}
}