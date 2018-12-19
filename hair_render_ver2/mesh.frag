#version 150 core
#extension GL_ARB_explicit_attrib_location: enable

layout(location = 0)out vec4 fragment;

//�ގ����
uniform vec3 kdiff;			//�g�U���ˌW��
uniform vec3 kspec;			//���ʔ��ˌW��
uniform vec3 kamb;			//�����ˌW��
uniform float kshi;			//�P���W��
uniform float transparency;	//���ߓx

// �e�N�X�`�����W�̃T���v��
uniform sampler2D dmap;		//�A���x�h
uniform sampler2D nmap;		//�m�[�}��
uniform sampler2D smap;		//�X�y�L����
uniform samplerCube cubemap;//��
uniform mat4 texFlag;		//�e�N�X�`���̃t���O

//in�ϐ�
in vec3 nl;		//�����x�N�g��
in vec3 lc;		//���̐F
in vec3 nv;		//�����x�N�g��
in vec3 nn;		//�@���x�N�g��
in vec2 tc;		//uv���W

void main()
{

	//�e�N�X�`�����Z�o
	vec4 Dcolor = vec4(kdiff,1.0);
	vec4 Scolor = vec4(kspec,1.0);
	vec3 Ncolor = vec3(0.0f);
	if(texFlag[0][0] == 1){ Dcolor = texture(dmap,tc); }
	if(texFlag[0][1] == 1){ Scolor = texture(smap,tc); }
	if(texFlag[0][2] == 1){ Ncolor = texture(nmap,tc).xyz * 2.0f - 1.0f; }
	
	//2�F���ː����f��(Blin)
	vec3 nh = normalize(nl + nv);			//���ԃx�N�g��
	vec3 normal = normalize(Ncolor + nn);	//�@���x�N�g��
	vec3 Idiff = max(dot(normal,nl),0) * lc * Dcolor.rgb;			//�g�U���ˌ�
	vec3 Ispec = pow(max(dot(normal,nh),0),kshi) * lc * Scolor.rgb;	//���ʔ��ˌ�
	vec3 Iamb = kamb * lc;											//����
	float a = Dcolor.a * transparency;								//�����x

	//�ŏI�I�ȐF���Z�o
	vec4 outColor = vec4(Idiff + Ispec + Iamb, a);
	fragment = outColor;	//�o��
	
}