#pragma once
#include"hr_math.h"

//�Q�[���I�u�W�F�N�g�̃X�[�p�[�N���X
class GameObject
{
private:
	std::string name;									//���O
	bool select = false;						//�I�����
	bool active = true;							//active���
	float position[3] = { 0.0f,0.0f,0.0f };		//�ʒu
	float rotation[3] = { 0.0f,0.0f,0.0f };		//�p��
	float scale[3] = { 1.0f,1.0f,1.0f };		//�g�k
	float preposition[3] = { 0.0f,0.0f,0.0f };	//1�X�e�b�v�O�̈ʒu
public:
	//�R���X�g���N�^(�������Ȃ�)
	GameObject() {}

	//�f�X�g���N�^(�������Ȃ�)
	~GameObject() {}

	//���z�֐�
	virtual void init() {}
	virtual void shutdown() {}
	virtual void update() {}

	//�f�X�g���C�֐�
	void destroy() { active = false; }

	//get�֐�
	std::string& getName();
	const float* getPosition();
	const float* getRotation();
	const float* getScale();
	const bool getActive();
	const bool getSelect();

	//set�֐�
	void onSelect();
	void offSelect();
	void setName(const char* n);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void addPosition(float x, float y, float z);
	void addRotation(float x, float y, float z);
	void addScale(float x, float y, float z);
	void setPrePosition();
};