#version 150 core
#extension GL_ARB_explicit_attrib_location: enable 

//attribute変数
layout(location = 0) in vec4 position;	//頂点座標
layout(location = 1) in vec4 normal;	//法線情報
layout(location = 2) in vec2 texcoord;	//uv座標

//行列類
uniform mat4 model;		//model行列
uniform mat4 view;		//view行列
uniform mat4 proj;		//projection行列
uniform mat4 adjoint;	//随伴行列

//light情報
uniform vec3 lpos;			//光の位置
uniform vec3 lvector;		//光の向き
uniform vec3 lcolor;		//光の色
uniform vec2 lsize;			//Areaライトの面積
uniform float lenergy;		//光のエネルギー
uniform float ldistance;	//光が届く範囲
uniform float lspot_size;	//スポットサイトの広さ
uniform float lblend;		//ぼかし具合
uniform float lgamma;		//
uniform samplerCube cubemap;

//光のタイプ
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;
int Enviroment = 5;

//out変数
out vec3 nl;	//光線ベクトル
out vec3 lc;	//光の色
out vec3 nv;	//視線ベクトル
out vec3 nn;	//法線ベクトル
out vec2 tc;	//uv座標
out mat4 tex;

//光のタイプによってベクトルを変える
vec3 setLightTypeVector(vec3 p)
{
	vec3 nl = vec3(0.0f);
	if(ltype == Point){
		nl = normalize(lpos - p);
	}else if(ltype == Enviroment)
	{
		nl = normalize(lpos - p);
		tex = transpose(view);
		tex[3][0] = tex[3][1] = tex[3][2] = 0.0;
	}
	else{
		nl = normalize(lvector);
	}
	return nl;
}

void main()
{
	//fragmentshaderへ送る
	tc = texcoord;

	vec4 pm = model * position;

	//正しい頂点座標を算出
	vec4 pv = view * pm;

	
	//正しい法線情報を算出
	nn = normalize((adjoint * normal).xyz);

	//視線ベクトルを算出
	nv = (-normalize(pv.xyz/pv.w));

	nl = setLightTypeVector(pm.xyz);
	lc  = lcolor;

	vec4 out_put = proj * pv;

	gl_Position = out_put;
}