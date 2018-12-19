#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec4 position;	//í∏ì_ç¿ïW

uniform float knots;

void main()
{
	gl_Position = position;
}