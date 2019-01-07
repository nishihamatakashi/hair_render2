#version 150 core
#extension GL_ARB_explicit_attrib_location : enable


//uniform材質
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;

//uniformボリュームデータ
uniform sampler3D vmap;

//uniform行列
uniform mat4 clip;
uniform mat4 texMatrix;
uniform mat4 adjoint;

//uniformパラメータ
uniform float threshold;
uniform float transparency; 
// in変数
in vec4 pv;	// スライスの表面上の位置
in vec3 tv;	// テクスチャ座標
in vec3 nl;

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fragment;

void main()
{
  // 元のボリュームの濃度と閾値の差
  float v = texture(vmap, tv).r * 0.1f;

  // 濃度が閾値以下ならフラグメントを捨てる
  if (v < threshold) discard;

   // 濃度の勾配を求める
  vec3 g = vec3(
    textureOffset(vmap, tv, ivec3(-1, 0, 0)).r - textureOffset(vmap, tv, ivec3(1, 0, 0)).r,
    textureOffset(vmap, tv, ivec3(0, -1, 0)).r - textureOffset(vmap, tv, ivec3(0, 1, 0)).r,
    textureOffset(vmap, tv, ivec3(0, 0, -1)).r - textureOffset(vmap, tv, ivec3(0, 0, 1)).r
  );

  
  vec3 l = nl; // 光線ベクトル
  vec3 n = normalize((adjoint * vec4(g,1.0)).xyz);       // 法線ベクトル
  vec3 h = normalize(l - normalize(pv.xyz));			 // 中間ベクトル
  
  // 拡散反射光＋環境光の反射光
  vec3 Idiff = max(dot(n, l), 0.0) * kdiff;
  
  // 鏡面反射光
  vec3 Ispec = pow(max(dot(n, h), 0.0), kshi) * kspec;

  // フラグメントの色
  fragment = vec4((Idiff + Ispec + kamb).rgb, v * transparency);
}