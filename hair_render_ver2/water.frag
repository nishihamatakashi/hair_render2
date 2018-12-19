#version 150 core
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_NV_shadow_samplers_cube: enable

//uniform材質
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;

//uniformボリュームデータ
uniform sampler3D vmap;
uniform samplerCube cubemap;

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
in vec3 lc;

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fragment;
float eta = 0.15f;
void main()
{
  // 元のボリュームの濃度と閾値の差
  float v = texture(vmap, tv).g * 0.01f;

  // 濃度が閾値以下ならフラグメントを捨てる
  if (v < threshold) discard;

   // 濃度の勾配を求める
  vec3 g = vec3(
    textureOffset(vmap, tv, ivec3(-1, 0, 0)).g - textureOffset(vmap, tv, ivec3(1, 0, 0)).g,
    textureOffset(vmap, tv, ivec3(0, -1, 0)).g - textureOffset(vmap, tv, ivec3(0, 1, 0)).g,
    textureOffset(vmap, tv, ivec3(0, 0, -1)).g - textureOffset(vmap, tv, ivec3(0, 0, 1)).g
  );

  vec3 nv = -normalize(pv.xyz);
  vec3 l = nl; // 光線ベクトル
  vec3 n = normalize((adjoint * vec4(g,1.0)).xyz);       // 法線ベクトル
  vec3 h = normalize(l - normalize(pv.xyz));			 // 中間ベクトル

  vec3 s = -(texMatrix * vec4(refract(nv,n,eta),1.0)).xyz;

  // 拡散反射光＋環境光の反射光
  vec3 Idiff = textureCube(cubemap,s).rgb;

  // 鏡面反射光
  vec3 Ispec = pow(max(dot(n, h), 0.0), kshi) * lc;

  // フラグメントの色
  fragment = vec4((Idiff + Ispec).rgb, transparency);
 }