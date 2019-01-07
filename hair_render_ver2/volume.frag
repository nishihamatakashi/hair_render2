#version 150 core
#extension GL_ARB_explicit_attrib_location : enable


//uniform�ގ�
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;

//uniform�{�����[���f�[�^
uniform sampler3D vmap;

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

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fragment;

void main()
{
  // ���̃{�����[���̔Z�x��臒l�̍�
  float v = texture(vmap, tv).r * 0.1f;

  // �Z�x��臒l�ȉ��Ȃ�t���O�����g���̂Ă�
  if (v < threshold) discard;

   // �Z�x�̌��z�����߂�
  vec3 g = vec3(
    textureOffset(vmap, tv, ivec3(-1, 0, 0)).r - textureOffset(vmap, tv, ivec3(1, 0, 0)).r,
    textureOffset(vmap, tv, ivec3(0, -1, 0)).r - textureOffset(vmap, tv, ivec3(0, 1, 0)).r,
    textureOffset(vmap, tv, ivec3(0, 0, -1)).r - textureOffset(vmap, tv, ivec3(0, 0, 1)).r
  );

  
  vec3 l = nl; // �����x�N�g��
  vec3 n = normalize((adjoint * vec4(g,1.0)).xyz);       // �@���x�N�g��
  vec3 h = normalize(l - normalize(pv.xyz));			 // ���ԃx�N�g��
  
  // �g�U���ˌ��{�����̔��ˌ�
  vec3 Idiff = max(dot(n, l), 0.0) * kdiff;
  
  // ���ʔ��ˌ�
  vec3 Ispec = pow(max(dot(n, h), 0.0), kshi) * kspec;

  // �t���O�����g�̐F
  fragment = vec4((Idiff + Ispec + kamb).rgb, v * transparency);
}