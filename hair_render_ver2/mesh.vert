#version 150 core
#extension GL_ARB_explicit_attrib_location: enable 

//attribute�ϐ�
layout(location = 0) in vec4 position;	//���_���W
layout(location = 1) in vec4 normal;	//�@�����
layout(location = 2) in vec2 texcoord;	//uv���W

//�s���
uniform mat4 model;		//model�s��
uniform mat4 view;		//view�s��
uniform mat4 proj;		//projection�s��
uniform mat4 adjoint;	//�����s��

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
uniform samplerCube cubemap;

//���̃^�C�v
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;
int Enviroment = 5;

//out�ϐ�
out vec3 nl;	//�����x�N�g��
out vec3 lc;	//���̐F
out vec3 nv;	//�����x�N�g��
out vec3 nn;	//�@���x�N�g��
out vec2 tc;	//uv���W
out mat4 tex;

//���̃^�C�v�ɂ���ăx�N�g����ς���
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
	//fragmentshader�֑���
	tc = texcoord;

	vec4 pm = model * position;

	//���������_���W���Z�o
	vec4 pv = view * pm;

	
	//�������@�������Z�o
	nn = normalize((adjoint * normal).xyz);

	//�����x�N�g�����Z�o
	nv = (-normalize(pv.xyz/pv.w));

	nl = setLightTypeVector(pm.xyz);
	lc  = lcolor;

	vec4 out_put = proj * pv;

	gl_Position = out_put;
}