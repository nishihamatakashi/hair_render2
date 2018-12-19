#version 150 core
#extension GL_ARB_explicit_attrib_location: enable 

layout(location = 0) in vec4 position;	//���_���W

uniform mat4 view;		//view�s��
uniform mat4 proj;		//projection�s��
uniform mat4 adjoint;
out vec3 vtc;

void main()
{
	vtc =  -(position).xyz;
	gl_Position = proj * view * position;
}