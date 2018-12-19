#version 150 core
#extension GL_ARB_explicit_attrib_location : enable


//行列情報
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 texMatrix;

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

//光のタイプ
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

//uniformパラメータ
uniform int slices;

//[-0.5 ,0.5]の正方形の頂点位置
layout(location= 0 ) in vec4 position;

//out変数
out vec4 pv;	//スライスの頂点位置
out vec3 tv;	//スライスのテクスチャ座標
out vec3 nl;	//光ベクトル
out vec3 lc;

//光のタイプによってベクトルを変える
vec3 setLightTypeVector(vec3 p)
{
	vec3 nl = vec3(0.0f);
	if(ltype == Point){
		nl = normalize(lpos - p);
	}else{
		nl = normalize(lvector);
	}
	return nl;
}

void main()
{
	lc = lcolor;
	float spacing = 2.0 / float( slices);

	//スライスをgl_InstanceIDでずらす
	float sliceID = float(slices - gl_InstanceID);
	vec4 t_pv = vec4(position.x , position.y, (sliceID - float( slices) * 0.5f) * spacing, 1.0);

	nl = setLightTypeVector((model * t_pv).xyz);

	//スライスのテクスチャ座標はスライスの中心を基準に√３倍に拡大してから回転する
	tv = (mat3(texMatrix) * t_pv.xyz) * 0.5 * 1.732 + 0.5;

	//頂点位置を視点座標系に移す
	pv = view * model * t_pv;

	//モデルビュープロジェクション変換をしてからラスタライザに送る
	gl_Position = proj * pv;
}