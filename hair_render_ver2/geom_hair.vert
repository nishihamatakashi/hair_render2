#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec4 position;	//���_���W

uniform float knots;

void main()
{
	gl_Position = position;
}