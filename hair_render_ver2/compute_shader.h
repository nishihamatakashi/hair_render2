#pragma once
#include"shader.h"

class ComputeShader
{
protected:
	std::vector<std::unique_ptr<Shader>> compute_shaders;
public:
	void addComputeShader(const char* comp);
	~ComputeShader() {}

	void loadUnilocate(size_t programID, size_t unilocateID, const char *name, const char *code);
	virtual void init(){}
	virtual void update(){}
	virtual void shutdown();
};

class TestComputeShader : public ComputeShader
{
public:
	void createComputeShader(const char *comp);
	void setUniform(Basic3D &object);
	void update(Basic3D& object);
};

class HairFTLShader : public ComputeShader
{
public:
	void createComputeShader(const char *comp0, const char *comp1);
	void setUniform(Basic3D &object, int programID,const float *p);
	void update(Basic3D& object,const float *p);
};


class BitnicSortShader : public ComputeShader
{
public:
	void createComputeShader(const char *comp0);
	void setUniform(GLuint tex0, GLuint tex1, int progamID, int block, int step, int offset, const int *size);
	void update(GLuint tex0, GLuint tex1, const int* size);
};