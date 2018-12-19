#version 150 core
#extension GL_ARB_explicit_attrib_location: enable

layout(location = 0)out vec4 fragment;

uniform mat4 view;
uniform mat4 proj;

//light���
uniform vec3 lpos;
uniform vec3 lvector;
uniform vec3 lcolor;

uniform vec2 lsize;

uniform float lenergy;
uniform float ldistance;

uniform float lspot_size;
uniform float lblend;
uniform float lgamma;

//0�FPoint,1�FSpot,2�FArea,3�FHemi,4�FSun
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

//�ގ����
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;
uniform float transparency;

// �e�N�X�`�����W�̃T���v��
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
	
	// Kajiya-Kay ���f���ɂ��A�e�t��
	float lt = dot(nl, ntt);
	float lt2 = sqrt(1.0 - lt * lt);
	float vt = dot(nv, ntt);
	float vt2 = sqrt(1.0 - vt * vt);

	//�e�N�X�`�����Z�o
	vec4 Dcolor = vec4(kdiff,1.0);
	if(texFlag[0][0] == 1){ Dcolor = texture(dmap,tc); }

	//�����x
	float alpha = Dcolor.a * transparency;

	//�g�U���ˌ����v�Z
	vec3 Idiff = lt2  * Dcolor.rgb * lc;
	
	//���ʔ��ˌ����v�Z
	vec3 Ispec =pow(max(lt2 * vt2 - lt * vt, 0.0), kshi) * lc * kspec;
	
	//�������v�Z
	vec3 Iamb = kamb * lc;

	//�ŏI�I�ȐF���Z�o
	vec4 output = vec4(Idiff + Ispec + Iamb,alpha);

	fragment = output;
}