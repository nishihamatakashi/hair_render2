#version 150 core
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_NV_shadow_samplers_cube: enable

layout(location = 0)out vec4 fragment;

//光のタイプ
uniform int ltype;
int Enviroment = 5;

//材質情報
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;
uniform float transparency;
uniform float roughness;

// テクスチャ座標のサンプラ
uniform sampler2D dmap;		// diffuse map
uniform sampler2D nmap;		// normal map
uniform sampler2D smap;		// specular map
uniform samplerCube cubemap;//cube map
uniform mat4 texFlag;

//in変数
in vec3 nl;
in vec3 lc;
in vec3 nv;
in vec3 nn;
in vec2 tc;
in mat4 tex;

const float PI = 3.141592;
const float EPSILON = 0.000001;

//ランバート拡散BRDF
vec3 DiffuseBRDF(vec3 diffcolor)
{
	return diffcolor / PI;
}

//フレネル係数(Schlick近似)
vec3 F_Schlick(vec3 speccolor,vec3 h,vec3 v)
{
	return (speccolor + (1.0 - speccolor) * pow((1.0 - max(dot(v,h),0)),5));
}

//マイクロファセット分布関数
float D_GXX(float a,float dotNH)
{

	float a2 = a * a;
	float dotNH2 = dotNH * dotNH;
	float d = dotNH2 * (a2 - 1.0) + 1.0;
	return a2 / (PI * d * d);
}

//幾何減衰項(smithモデル + schlick近似)
float G_Smith_Schlick_GGX(float a,float dotNV,float dotNL)
{
	float k = a * a * 0.5 + EPSILON;
	float gl = dotNL / (dotNL * (1.0 - k) + k);
	float gv = dotNV / (dotNV * (1.0 - k) + k);
	return gl * gv;
}

//Cook-Torrance
vec3 SpecularBRDF(vec3 speccolor,vec3 N, vec3 V,vec3 L)
{
	float dotNL = max(dot(N,L),0);
	float dotNV = max(dot(N,V),0);
	vec3 H = normalize(L + V);
	float dotNH = max(dot(N,H),0);
	float dotVH = max(dot(V,H),0);
	float dotLV = max(dot(L,V),0);
	float a = roughness * roughness;

	float D = D_GXX(a,dotNH);
	float G = G_Smith_Schlick_GGX(a,dotNV,dotNL);
	vec3 F =  F_Schlick(speccolor,V,H);

	return (F*(G*D))/(4.0 * dotNL * dotNV + EPSILON);
}

void main()
{
	//テクスチャを算出
	vec4 Dcolor = vec4(kdiff,1.0);
	vec4 Scolor = vec4(kspec,1.0);
	vec3 Ncolor = vec3(0.0f);
	vec3 Ccolor = vec3(0.0f);
	if(texFlag[0][0] == 1){ Dcolor = texture(dmap,tc); }
	if(texFlag[0][1] == 1){ Scolor = texture(smap,tc); }
	if(texFlag[0][2] == 1){ Ncolor = texture(nmap,tc).xyz * 2.0f - 1.0f; }
	vec3 L = nl;
	vec3 N = normalize(nn + Ncolor);
	vec3 V = nv;

	vec3 r = (tex * vec4(reflect(N,V),1.0)).xyz;
	if(texFlag[1][2] == 1){ Ccolor = textureCube(cubemap,r).rgb;}

	float dotNL = max(dot(N,L),0);
	vec3 lco = lc;
	if(ltype == Enviroment)
	{
		lco = Ccolor;
	}
	vec3 irradiance = dotNL * lco;

	//punctual Light
	irradiance *= PI;

	vec3 Idiff = irradiance * DiffuseBRDF(Dcolor.rgb);
	vec3 Ispec = irradiance * SpecularBRDF(Scolor.rgb,N,V,L);
	vec3 Iamb = kamb.rgb;


	vec3 outColor = Idiff + Ispec + Iamb;
	fragment = vec4(outColor,Dcolor.a * transparency);	
}