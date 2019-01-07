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
	void setUniform(Basic3D &object, int programID, Fluid3D& fluid);
	void update(Basic3D& object, Fluid3D& fluid);
};


class BitnicSortShader : public ComputeShader
{
public:
	void createComputeShader(const char *comp0);
	void setUniform(GLuint tex0, GLuint tex1, int progamID, int block, int step, int offset, const int *size);
	void update(GLuint tex0, GLuint tex1, const int* size);
};


enum UPDATEFLAG
{
	Density,
	Velocity
};
class FluidShader : public ComputeShader
{
private:
	int LINEARSOLVERTIMES = 1;
public:
	void createComputeShader(const char *comp0, const char *comp1,
		const char *comp2, const char *comp3,
		const char *comp4, const char *comp5,
		const char *comp6);
	void swap(Fluid3D &object, UPDATEFLAG f)
	{
		if (f == Velocity) {
			GLuint tmp;
			tmp = object.getSimulate_tex()[0];
			object.getSimulate_tex()[0] = object.getSimulate_tex()[1];
			object.getSimulate_tex()[1] = tmp;
		}
		else if (f == Density) 
		{
			GLuint tmp;
			tmp = object.getSimulate_tex()[2];
			object.getSimulate_tex()[2] = object.getSimulate_tex()[3];
			object.getSimulate_tex()[3] = tmp;
		}
	}
	void setUniform(Fluid3D &object, UPDATEFLAG f, int programID, int b, float dt);
	void dispatch(Fluid3D &object, UPDATEFLAG f, int programID, int b, float dt);
	void updateVel(Fluid3D& object, const float dt);
	void updateDens(Fluid3D& object, const float dt);
	void update(Fluid3D& object, const float dt);
	void add_data(Fluid3D &data);
};