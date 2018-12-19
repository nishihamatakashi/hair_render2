#pragma once
#include "manager.h"
#include "window.h"

//hair_render�̊�{�N���X
class HR
{
public:

	//manager�̃C���X�^���X���쐬
	TextureManager texture_manager;
	MaterialManager material_manager;
	LightManager light_manager;
	Object3DManager object3d_manager;
	ShaderManager shader_manager;

	//singleton object�C���X�^���X�̍쐬
	Window window;
	Camera camera;
	ImGuiIO* io = nullptr;
	SkyBox skybox;
	//���̃V�X�e���̐ݒ�ϐ�
	const char* glsl_version = "#version 130";
	bool showgui = true;

	//����������
	bool init();

	//����X�V����֐�
	void update();

	//�C������
	void shutdown();

	//��������̃I�u�W�F�N�g�������֐�
	void object_move(Basic3D& obj);

	//window�̕������ǂ�������
	bool shouldClose();

	//GUI�̕\���֐�
	void showGUI();
	void showResource();
	void ObjectSetting();
};
