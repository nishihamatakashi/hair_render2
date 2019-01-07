#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec4 position;	//頂点座標
layout(location = 1) in vec4 velocity;	//速度

void main()
{
	gl_Position = position;
	gl_PointSize = velocity.w;
}