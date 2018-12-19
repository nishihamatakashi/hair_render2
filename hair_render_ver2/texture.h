#pragma once

#include"hr_math.h"

//�ʏ�̃J���[�}�b�v�ǂݍ���
extern void LoadImage(GLuint &tex, const char *name);

//�����}�b�v�̓ǂݍ���
extern void LoadHeight(GLuint &tex, const char *name, float nz);

//�L���[�u�}�b�v�ǂݍ���
extern void LoadCubeImage(GLuint &tex, const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz);

//�e�X�g�p
extern void LoadTestMap(GLuint &tex, const char *name);
