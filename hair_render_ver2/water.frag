#version 150 core
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_NV_shadow_samplers_cube: enable

//uniform�ގ�
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;

//uniform�{�����[���f�[�^
uniform sampler3D vmap;
uniform samplerCube cubemap;

//uniform�s��
uniform mat4 clip;
uniform mat4 texMatrix;
uniform mat4 adjoint;

//uniform�p�����[�^
uniform float threshold;
uniform float transparency; 
// in�ϐ�
in vec4 pv;	// �X���C�X�̕\�ʏ�̈ʒu
in vec3 tv;	// �e�N�X�`�����W
in vec3 nl;
in vec3 lc;

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fragment;
float eta = 0.15f;
void main()
{
  // ���̃{�����[���̔Z�x��臒l�̍�
  float v = texture(vmap, tv).g * 0.01f;

  // �Z�x��臒l�ȉ��Ȃ�t���O�����g���̂Ă�
  if (v < threshold) discard;

   // �Z�x�̌��z�����߂�
  vec3 g = vec3(
    textureOffset(vmap, tv, ivec3(-1, 0, 0)).g - textureOffset(vmap, tv, ivec3(1, 0, 0)).g,
    textureOffset(vmap, tv, ivec3(0, -1, 0)).g - textureOffset(vmap, tv, ivec3(0, 1, 0)).g,
    textureOffset(vmap, tv, ivec3(0, 0, -1)).g - textureOffset(vmap, tv, ivec3(0, 0, 1)).g
  );

  vec3 nv = -normalize(pv.xyz);
  vec3 l = nl; // �����x�N�g��
  vec3 n = normalize((adjoint * vec4(g,1.0)).xyz);       // �@���x�N�g��
  vec3 h = normalize(l - normalize(pv.xyz));			 // ���ԃx�N�g��

  vec3 s = -(texMatrix * vec4(refract(nv,n,eta),1.0)).xyz;

  // �g�U���ˌ��{�����̔��ˌ�
  vec3 Idiff = textureCube(cubemap,s).rgb;

  // ���ʔ��ˌ�
  vec3 Ispec = pow(max(dot(n, h), 0.0), kshi) * lc;

  // �t���O�����g�̐F
  fragment = vec4((Idiff + Ispec).rgb, transparency);
 }