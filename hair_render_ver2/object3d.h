#pragma once
#include"camera.h"
#include"material.h"

/*
�I�u�W�F�N�g3D�̃N���X��`
*/
class Object3D : public GameObject
{
private:
	BASIC3D_TYPE type;
	SHADER_TYPE stype;

	//draw flag
	bool cull_front = false;
	bool cull_back = true;
	bool depth_test = true;
	bool alpha_test = true;

	//billboad flag
	bool billboad = false;

	//matrix
	Matrix m;

	//material
	Material *material = nullptr;

	//�O��
	float force[3] = { 0.0f,0.0f,0.0f };
public:
	//constructor
	Object3D() {}

	//destructor
	~Object3D() {}

	void init()override {}
	void shutdown()override {}
	void update()override {}
	virtual float *getParameter() { return 0; }
	void setEnableFunc();

	//set function
	void onCull_Front();
	void onCull_Back();
	void onDepth_Test();
	void offDepth_Test();
	void onAlpha_Test();
	void offAlpha_Test();
	void onBillboad();
	void offBillboad();
	void setMatrix(Camera &camera);
	void setMaterial(Material &m);
	void setType(BASIC3D_TYPE t);
	void setSType(SHADER_TYPE st);
	void setForce(float x, float y, float z);
	//get function
	bool getCull_Front();
	bool getCull_Back();
	bool getDepth_Test();
	bool getAlpha_Test();
	Matrix* getMatrix();
	Material *getMaterial();
	BASIC3D_TYPE getType();
	const float *getForce();
};

/*
��{�I��3D�̃N���X��`
*/
class Basic3D : public Object3D
{
private:

	//count
	int vertices;
	int indices;

	GLuint vao;
	GLuint vbo[3];
	GLuint tbo[3];

	//tbo resolution
	int resolution[2];

	//any parameter
	float parameter[20];
	std::vector<float> vec_parameter;

public:

	std::vector<GLfloat> v;
	//��ԕ����p�̃f�[�^
	GLuint space_tex;
	//�R���X�g���N�^
	Basic3D() {}
	//�f�X�g���N�^
	~Basic3D() {};

	void loadObject3D(BASIC3D_TYPE t,SHADER_TYPE st);
	void loadObject3D(BASIC3D_TYPE t, SHADER_TYPE st, float hairs, float knots, float length, float radius);
	void init() override {}
	void shutdown()override;
	void update()override {}

	//get�֐�
	GLuint getVao();
	GLuint *getTbo();
	int getVertices();
	int getIndices();
	const int *getResolution();
	float *getParameter();
	std::vector<float>& getVec_parameter();
};

class Volume3D : public Object3D
{
private:
	int cell_size[3];
	int vertices;
	int indices;
	float parameter[10];
	GLuint vao;
	GLuint vbo;
	GLuint vdata;

public:

	void loadObject3D(BASIC3D_TYPE t, SHADER_TYPE st, int slices, float threshold, int size);
	void loadObject3D(GLuint tex, SHADER_TYPE st, int slices, float threshold, int size);
	void init() override {}
	void shutdown() override;

	//get�֐�
	const int *getCell_size();
	GLuint getVao();
	GLuint getVdata();
	int getVertices();
	float *getParameter();
};

class SkyBox
{
private:
	//count
	int vertices;	//���_��
	int indices;	//�C���f�b�N�X

	float view[16];		//�����view�ϊ��s��
	float proj[16];
	float adjo[16];
	float fovy = 30.0f;	//cubemap�p�̉�p
	GLuint vbo;			//vbo
	GLuint vao;			//vao
	GLuint tex;
public:
	//�R���X�g���N�^
	SkyBox() {};

	//�f�X�g���N�^
	~SkyBox() {};
	
	//
	void loadSkybox();
	void shutdown() 
	{
		glDeleteBuffers(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
	};
	//set�֐�
	void setEnableFunc();
	void setMatrix(Camera &camera);
	void setTex(GLuint t) { tex = t; }
	void setFovy(float f) { fovy = f; }
	//get�֐�
	const int getVertices() { return this->vertices; }
	const int getIndices() { return this->indices; }
	const float *getView() { return this->view; }
	const float *getProj() { return this->proj; }
	const float *getAdjo() { return this->adjo; }
	const GLuint getTex() { return this->tex; }
	const GLuint getVbo() { return this->vbo; }
	const GLuint getVao() { return this->vao; }
	const float getFovy() { return this->fovy; }
};