#pragma once

/*
** enum�錾�C�G���[�C�`�F�b�N����
*/

#include<iostream>
#include<fstream>
#include <algorithm>
#include <vector>
#include<map>
#include<memory>
#include <cassert>
#include <string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

//�����̃^�C�v
enum LIGHT_TYPE
{
	Point,
	Spot,
	Area,
	Hemi,
	Sun,
	Enviroment
};

//�J�����̃^�C�v
enum CAMERA_TYPE
{
	Perth,
	Ortho,
};

//shader�̃^�C�v
enum SHADER_TYPE 
{
	SMesh,
	SVolume,
	SHair,
	SPoint,
	SLine,
	SSkybox
};

//3D�I�u�W�F�N�g�̃^�C�v
enum BASIC3D_TYPE
{
	Plane,
	Cube,
	Sphere,
	Skybox,
	Billboad,
	SphereHair,
	PlaneHair,
	Volume,
	VRandom,
	VCube
};

//�e�N�X�`���̃^�C�v
enum TEXTURE_TYPE
{
	ColorMap,
	HeightMap,
	CubeMap,
	FboMap,
	TestMap
};

//���̃^�C�v���當�����Ԃ�
inline const char* checkLightType(LIGHT_TYPE t)
{
	switch (t)
	{
	case Point:
		return "Point";
		break;
	case Sun:
		return "Sun";
		break;
	case Spot:
		return "Spot";
		break;
	case Hemi:
		return "Hemi";
		break;
	case Area:
		return "Area";
		break;
	default:
		return "";
		break;
	}
}

//3D�I�u�W�F�N�g�̃^�C�v���當�����Ԃ�
inline const char* checkBasic3DType(BASIC3D_TYPE t)
{
	switch (t)
	{
	case Plane:
		return "Plane";
		break;
	case Cube:
		return "Cube";
		break;
	case Sphere:
		return "Sphere";
		break;
	case Skybox:
		return "Skybox";
		break;
	case Billboad:
		return "Billboad";
		break;
	case SphereHair:
		return "SphereHair";
	case PlaneHair:
		return "PlaneHair";
	case Volume:
		return "Volume";
	case VRandom:
		return "VRandom";
	case VCube:
		return "VCube";
	default:
		return "";
		break;
	}
}

//�e�N�X�`���̃^�C�v���當�����Ԃ�
inline const char* checkTextureType(TEXTURE_TYPE t)
{
	switch (t)
	{
	case ColorMap:
		return "ColorMap";
		break;
	case HeightMap:
		return "HeightMap";
		break;
	case CubeMap:
		return "CubeMap";
		break;
	case FboMap:
		return "FboMap";
		break;
	case TestMap:
		return "TestMap";
		break;
	default:
		return "";
		break;
	}
}

//unilocate�f�[�^����C��肭�擾�ł��ĂȂ����̂��G���[�\��
void inline ErrorUnilocate(GLint loc, const char* loc_name, const char *code)
{
	if (loc < 0)
	{
		std::cerr << "Check�FNot find unilocate " << "in " << code << " : " << loc_name << std::endl;
	}
}

//�������O�̃I�u�W�F�N�g�C�A�Z�b�g���Ȃ��`�F�b�N
template<typename T>inline bool checkName(const char *name, T& data)
{
	auto itr = data.find(name);
	if (itr != data.end())
	{
		return true;
	}
	else {
		return false;
	}
}

//�`���I�u�W�F�N�g�Ƀ}�e���A�����t�^����Ă��邩
template<typename T>inline void checkDrawObject(T &data)
{
	if (data.getMaterial() == nullptr) {
		std::cerr << " ERROR�Fthis object's material is nullptr �F" << data.getName().c_str() << std::endl;
		assert(false);
	}
}

//glsl���Copengl�Ŏ擾�����G���[���ڂ�\��
void inline _glError(const char *name, unsigned int line)
{
	const GLenum error(glGetError());

	if (error != GL_NO_ERROR)
	{
		if (name)
		{
			std::cerr << name;
			if (line > 0) std::cerr << " (" << line << ")";
			std::cerr << ": ";
		}

		switch (error)
		{
		case GL_INVALID_ENUM:
			std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cerr << "A numeric argument is out of range" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "The specified operation is not allowed in the current state" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << "There is not enough memory left to execute the command" << std::endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << "The specified operation is not allowed current frame buffer" << std::endl;
			break;
		default:
			std::cerr << "An OpenGL error has occured: " << std::hex << std::showbase << error << std::endl;
			break;
		}
	}
}

//�f�B�[�v�R�s�[�p�̊֐� vec3
void inline deepcopy_vec3(const float *p0, float *p1)
{
	p1[0] = p0[0];
	p1[1] = p0[1];
	p1[2] = p0[2];
}

//�f�B�[�v�R�s�[�p�̊֐� vec4
void inline deepcopy_vec4(const float *p0, float *p1)
{
	p1[0] = p0[0];
	p1[1] = p0[1];
	p1[2] = p0[2];
	p1[3] = p0[3];
}
