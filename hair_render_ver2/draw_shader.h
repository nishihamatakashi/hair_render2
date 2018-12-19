#pragma once
/*
描画系shaderのクラス定義
*/
#include"shader.h"

class PointShader : public Shader
{
public:
	void createShader(const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0);
	void setUniform(Basic3D &object, Light &light, Material &material);
	void draw(Basic3D& object, Light& light, Camera& camera);
};

class LineShader : public Shader
{
public:
	void createShader(const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0);
	void setUniform(Basic3D &object, Light &light, Material &material);
	void draw(Basic3D& object, Light& light, Camera& camera);
};

class MeshShader : public Shader
{
public:
	void createShader(const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0);
	void setUniform(Basic3D &object, Light &light, Material &material);
	void draw(Basic3D& object, Light& light, Camera& camera);
};


class HairShader : public Shader
{
public:
	void createShader(const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0);
	void setUniform(Basic3D &object, Light &light, Material &material);
	void draw(Basic3D& object, Light& light, Camera& camera);
};

class VolumeShader : public  Shader
{
public:
	void createShader(const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0);
	void setUniform(Volume3D &object, Light &light, Material &material);
	void draw(Volume3D& object, Light& light, Camera& camera);
};

class SkyboxShader : public Shader 
{
public:
	void createShader(const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0);
	void setUniform(SkyBox &object);
	void draw(SkyBox& object, Camera& camera);
};

/*
template<typename T>class FrameBufferObject
{
private:
	GLuint fb;
	GLuint cb;
	GLuint rb;
	GLuint db;
	int size[2] = { 1028,1028 };
	int window[2];
	T* shader;
public:

	FrameBufferObject() {}
	~FrameBufferObject() {}

	void loadFrameBuffer(int w, int h, int ww, int wh, T *s)
	{
		shader = s;
		size[0] = w;
		size[1] = h;
		window[0] = ww;
		window[1] = wh;
		//カラーバッファ用テクスチャの生成
		glGenTextures(1, &cb);
		glBindTexture(GL_TEXTURE_2D, cb);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		//デプスバッファ用テクスチャの生成
		glGenTextures(1, &db);
		glBindTexture(GL_TEXTURE_2D, db);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size[0], size[1], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glBindTexture(GL_TEXTURE_2D, 0);

		//フレームバッファの生成
		glGenFramebuffers(1, &fb);
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, db, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_tgError(__FILE__, __LINE__);
	}
	void shutdown() {
		glDeleteTextures(1, &cb);
		glDeleteTextures(1, &db);
		glDeleteFramebuffers(1, &fb);
	};

	void draw(const char *name, Basic3D& object, Light& light, Camera& camera)
	{
		glViewport(0, 0, size[0], size[1]);
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader->draw(name, object, light, camera);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window[0], window[1]);
	}

	//get function
	GLuint getFrameBuffer() { return fb; }
	GLuint getColorBuffer() { return cb; }
	GLuint getRenderBuffer() { return rb; }
	GLuint getDepthBuffer() { return db; }
};
*/