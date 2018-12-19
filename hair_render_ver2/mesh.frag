#version 150 core
#extension GL_ARB_explicit_attrib_location: enable

layout(location = 0)out vec4 fragment;

//材質情報
uniform vec3 kdiff;			//拡散反射係数
uniform vec3 kspec;			//鏡面反射係数
uniform vec3 kamb;			//環境反射係数
uniform float kshi;			//輝き係数
uniform float transparency;	//透過度

// テクスチャ座標のサンプラ
uniform sampler2D dmap;		//アルベド
uniform sampler2D nmap;		//ノーマル
uniform sampler2D smap;		//スペキュラ
uniform samplerCube cubemap;//環境
uniform mat4 texFlag;		//テクスチャのフラグ

//in変数
in vec3 nl;		//光線ベクトル
in vec3 lc;		//光の色
in vec3 nv;		//視線ベクトル
in vec3 nn;		//法線ベクトル
in vec2 tc;		//uv座標

void main()
{

	//テクスチャを算出
	vec4 Dcolor = vec4(kdiff,1.0);
	vec4 Scolor = vec4(kspec,1.0);
	vec3 Ncolor = vec3(0.0f);
	if(texFlag[0][0] == 1){ Dcolor = texture(dmap,tc); }
	if(texFlag[0][1] == 1){ Scolor = texture(smap,tc); }
	if(texFlag[0][2] == 1){ Ncolor = texture(nmap,tc).xyz * 2.0f - 1.0f; }
	
	//2色反射性モデル(Blin)
	vec3 nh = normalize(nl + nv);			//中間ベクトル
	vec3 normal = normalize(Ncolor + nn);	//法線ベクトル
	vec3 Idiff = max(dot(normal,nl),0) * lc * Dcolor.rgb;			//拡散反射光
	vec3 Ispec = pow(max(dot(normal,nh),0),kshi) * lc * Scolor.rgb;	//鏡面反射光
	vec3 Iamb = kamb * lc;											//環境光
	float a = Dcolor.a * transparency;								//透明度

	//最終的な色を算出
	vec4 outColor = vec4(Idiff + Ispec + Iamb, a);
	fragment = outColor;	//出力
	
}