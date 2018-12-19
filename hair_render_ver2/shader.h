#pragma once
#include "object3d.h"
#include "Light.h"

/*
shaderのスーパークラスの定義
*/

//shaderのスーパークラス
class Shader
{
private:
protected:
	GLuint program = 0;

	//uniform location
	GLint unilocate[40] = {};
public:
	void loadShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse);
	void loadShader(const char* comp);

	Shader() {}
	~Shader() {}
	virtual void init(){}
	virtual void update(){}
	virtual void shutdown();
	void loadUnilocate(size_t n, const char *name, const char *code);
	GLuint getProgram();
	GLint *getUnilocate();
};