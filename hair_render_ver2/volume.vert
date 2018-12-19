#version 150 core
#extension GL_ARB_explicit_attrib_location : enable


//�s����
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 texMatrix;

//light���
uniform vec3 lpos;			//���̈ʒu
uniform vec3 lvector;		//���̌���
uniform vec3 lcolor;		//���̐F
uniform vec2 lsize;			//Area���C�g�̖ʐ�
uniform float lenergy;		//���̃G�l���M�[
uniform float ldistance;	//�����͂��͈�
uniform float lspot_size;	//�X�|�b�g�T�C�g�̍L��
uniform float lblend;		//�ڂ����
uniform float lgamma;		//

//���̃^�C�v
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

//uniform�p�����[�^
uniform int slices;

//[-0.5 ,0.5]�̐����`�̒��_�ʒu
layout(location= 0 ) in vec4 position;

//out�ϐ�
out vec4 pv;	//�X���C�X�̒��_�ʒu
out vec3 tv;	//�X���C�X�̃e�N�X�`�����W
out vec3 nl;	//���x�N�g��
out vec3 lc;

//���̃^�C�v�ɂ���ăx�N�g����ς���
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

	//�X���C�X��gl_InstanceID�ł��炷
	float sliceID = float(slices - gl_InstanceID);
	vec4 t_pv = vec4(position.x , position.y, (sliceID - float( slices) * 0.5f) * spacing, 1.0);

	nl = setLightTypeVector((model * t_pv).xyz);

	//�X���C�X�̃e�N�X�`�����W�̓X���C�X�̒��S����Ɂ�R�{�Ɋg�債�Ă����]����
	tv = (mat3(texMatrix) * t_pv.xyz) * 0.5 * 1.732 + 0.5;

	//���_�ʒu�����_���W�n�Ɉڂ�
	pv = view * model * t_pv;

	//���f���r���[�v���W�F�N�V�����ϊ������Ă��烉�X�^���C�U�ɑ���
	gl_Position = proj * pv;
}