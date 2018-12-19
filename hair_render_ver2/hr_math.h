#pragma once

/*
hair_render�Ŏg�����w���C����
*/

#include "hr_type.h"

//x��low�`high�ŃN�����v
#define CLAMP(x, low, high) (x > high)? high : (x < low)? low : x

//�s����
typedef struct
{
	float model[16];
	float view[16];
	float proj[16];
	float adjoint[16];
	float clip[16];
	float texture[16];
}Matrix;


//���鐔������e�N�X�`���Ɋi�[�����Ƃ��̃T�C�Y���v�Z(2^n��)
extern void calcResolution(const int n, int* size);

//�}�E�X��(2D)�ړ�����ړ��������Z�o����
extern void mouseTranslate(float x, float y, float *dir, float *p);

//�}�E�X��(2D)�ړ������]���Z�o����
extern void mouseRotate(float x, float y, float *dir, float *p0, float *p1);

//�}�E�X�̃z�C�[���ړ�����ړ��������Z�o����
extern void mouseZoom(float w, float *dir, float *p);

//���W�l�����ԓo�^����n�b�V���֐�
extern int setSpace_Hash(float x, float y, float z, int cell_num, float length);

//�����֐�
extern float distance(float x, float y, float z);

//���K���֐�
extern void Normalize(float *p);

//��]�s��ł͂Ȃ��C���W���Z�o
extern void loadRotateSimple(float *p, float *p1, float *n, float a);

//���ʐ��`�ۊ�
extern float *slerp(float *q1, float *q2, float *q3, float t);

//�N�H�[�^�j�I�����Z�o
extern float *qmake(float *n, float a, float *q);

//�P�ʍs��
extern float *loadIdentity(float *m);

//�s��̊|���Z
extern float *multiply(const float *m0, const float *m1, float *m);

//�����s��̎Z�o
extern float *adjoint(float *m0, float *m1);


//�ړ�������s����Z�o(m���P�ʍs��̎�)
extern float *loadTranslate(float x, float y, float z, float *m);

//�N�H�[�^�j�I�����v�Z(m���P�ʍs��̎�)
extern float *loadQuaternion(float *q, float *m);

//�C�ӎ���](m���P�ʍs��̎�)
extern float *loadRotate(float a, float *n, float *m);

//�I�C���[�p��](m���P�ʍs��̎�)
extern float *loadEulerRotate(const float *r, float *m);

//�g��k��(m���P�ʍs��̎�)
extern float *loadScale(float x, float y, float z, float *m);

//�r���[�ϊ��s����Z�o(m���P�ʍs��̎�)
extern float *loadLookat(const float *e, const float *g, const float *u, float *m);

//�r���[�ϊ��s��(Skybox�p)���Z�o(m���P�ʍs��̎�)
extern float *loadSkyLookat(const float *e,const float *g,const float *u, float *rv);

//�������e�ϊ��s����Z�o(m���P�ʍs��̎�)
extern float *loadOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar, float *m);

//�������e�ϊ��s����Z�o(m���P�ʍs��̎�)
extern float *loadFrustum(float left, float right, float bottom, float top, float zNear, float zFar, float *m);

//��p��p���������ϊ��s����Z�o(m���P�ʍs��̎�)
extern float *loadPerspective(float fovy, float aspect, float zNear, float zFar, float *m);

//�ړ�������s����Z�o
extern float *Translate(float x, float y, float z, float *m);

//�g��k��(m���P�ʍs��̎�)
extern float *Scale(float x, float y, float z, float *m);

//�I�C���[�p��]
extern float *EulerRotate(const float *r, float *m);

//�C�ӎ���]
extern float *Rotate(float a, float *n, float *m);

//�r���[�ϊ��s����Z�o
extern float *Lookat(float *e, float *g, float *u, float *m);

//�N�H�[�^�j�I�����v�Z
extern float *Quaternion(float *q, float *m);

//�t�s����Z�o����
extern float *Inverse(float *m0, float *m1);

//�I�u�W�F�N�g�ɂ����񂩂�model�s����Z�o����
extern float *CalcModelMatrix(float *t, float *r, float *s, float *m);