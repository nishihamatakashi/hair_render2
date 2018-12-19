#pragma once

/*
ƒ}ƒeƒŠƒAƒ‹‚ÌƒNƒ‰ƒX’è‹`
*/

#include "hr_math.h"

class Material
{
private:
	float kdiff[3] = { 0.8f,0.8f, 0.8f};	//ŠgŽU”½ŽËŒW”
	float kspec[3] = { 0.4f, 0.4f, 0.4f};	//‹¾–Ê”½ŽËŒW”
	float kamb[3] = { 0.1f, 0.1f, 0.1f};	//ŠÂ‹«”½ŽËŒW”
	float color[3] = { 0.0f,0.0f,0.0f};		//F
	float kshi = 40.0f;						//‹P‚«ŒW”
	float roughness = 1.0f;					//‘e‚³ŒW”
	float transparency = 1.0f;				//“§–¾“x

	//texture
	//0FalbedoC1:normalC2FspecularC3FambientC4Fdepth, 5FcolorC6FcubeC7`F"free"
	float tex_flag[16] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	GLuint tex[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	const char* tex_name[16];
public:

	//constructor
	Material() {}

	//destructor
	~Material() {};

	void init() {}
	void shutdown() {}
	void update() {}

	//set function
	void setkDiff(float r, float g, float b) {
		kdiff[0] = r;
		kdiff[1] = g;
		kdiff[2] = b;
	}
	void setKSpec(float r, float g, float b) {
		kspec[0] = r;
		kspec[1] = g;
		kspec[2] = b;
	}
	void setKAmb(float r, float g, float b) {
		this->kamb[0] = r;
		this->kamb[1] = g;
		this->kamb[2] = b;
	}
	void setColor(float r, float g, float b) {
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
	void setTexture(size_t n, GLuint t, const char* na) {
		if (n < 16) {
			tex[n] = t;
			tex_flag[n] = 1.0f;
			tex_name[n] = na;
		}
	}
	void setKshi(float e) { this->kshi = e; };
	void setRoughness(float r) { this->roughness = r; }
	void setTransparency(float t) { this->transparency = t; }
	void setAlbedoMap(GLuint t, const char* n) { setTexture(0, t,n); }
	void setNormalMap(GLuint t, const char* n) { setTexture(1, t, n); }
	void setSpecularMap(GLuint t, const char* n) { setTexture(2, t, n); }
	void setAmbientMap(GLuint t, const char* n) { setTexture(3, t, n); }
	void setDepthMap(GLuint t, const char* n) { setTexture(4, t, n); }
	void setColorMap(GLuint t, const char* n) { setTexture(5, t, n); }
	void setCubeMap(GLuint t, const char* n) { setTexture(6, t, n); }

	//get function
	GLuint getTexture(int n) {
		if (n < 16 && n > -1) {
			return tex[n];
		}
		else {
			return NULL;
		}
	}
	const char* getTextureName(int n) {
		if (n < 16 && n > -1) {
			return tex_name[n];
		}
		else {
			return NULL;
		}
	}
	float getKshi() { return this->kshi; };
	float getRoughness() { return this->roughness; }
	float getTransparency() { return this->transparency; }
	float *getKDiff() { return this->kdiff; };
	float *getKSpec() { return this->kspec; };
	float *getKAmb() { return this->kamb; };
	float *getcolor() { return this->color; };
	GLuint getAlbedoMap(){ return getTexture(0); }
	GLuint getNormalMap() { return getTexture(1); }
	GLuint getSpecularMap() { return getTexture(2); }
	GLuint getAmbientMap() { return getTexture(3); }
	GLuint getDepthMap() { return getTexture(4); }
	GLuint getColorMap() { return getTexture(5); }
	GLuint getCubeMap() { return getTexture(6); }

	const char* getAlbedoMapName() { return getTextureName(0); }
	const char* getNormalMapName() { return getTextureName(1); }
	const char* getSpecularMapName() { return getTextureName(2); }
	const char* getAmbientMapName() { return getTextureName(3); }
	const char* getDepthMapName() { return getTextureName(4); }
	const char* getColorMapName() { return getTextureName(5); }
	const char* getCubeMapName() { return getTextureName(6); }
	const float *getTex_Flag() { return tex_flag; }
};