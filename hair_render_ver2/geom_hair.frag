#version 150 core
#extension GL_ARB_explicit_attrib_location: enable

layout(location = 0)out vec4 fragment;

uniform mat4 view;
uniform mat4 proj;

//light情報
uniform vec3 lpos;
uniform vec3 lvector;
uniform vec3 lcolor;

uniform vec2 lsize;

uniform float lenergy;
uniform float ldistance;

uniform float lspot_size;
uniform float lblend;
uniform float lgamma;

//0：Point,1：Spot,2：Area,3：Hemi,4：Sun
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

//材質情報
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;
uniform float transparency;

// テクスチャ座標のサンプラ
uniform sampler2D dmap;		// diffuse map
uniform sampler2D nmap;		// normal map
uniform sampler2D smap;		// specular map
uniform sampler2D depthmap;
uniform samplerCube cubemap;//cube map
uniform mat4 texFlag;

in vec2 tc;
in vec3 nl;
in vec3 nv;
in vec3 ntt;

void main()
{
	vec3 lc = lcolor;
	
	// Kajiya-Kay モデルによる陰影付け
	float lt = dot(nl, ntt);
	float lt2 = sqrt(1.0 - lt * lt);
	float vt = dot(nv, ntt);
	float vt2 = sqrt(1.0 - vt * vt);

	//テクスチャを算出
	vec4 Dcolor = vec4(kdiff,1.0);
	if(texFlag[0][0] == 1){ Dcolor = texture(dmap,tc); }

	//透明度
	float alpha = Dcolor.a * transparency;

	//拡散反射光を計算
	vec3 Idiff = lt2  * Dcolor.rgb * lc;
	
	//鏡面反射光を計算
	vec3 Ispec =pow(max(lt2 * vt2 - lt * vt, 0.0), kshi) * lc * kspec;
	
	//環境光を計算
	vec3 Iamb = kamb * lc;

	//最終的な色を算出
	vec4 output = vec4(Idiff + Ispec + Iamb,alpha);

	fragment = output;
}