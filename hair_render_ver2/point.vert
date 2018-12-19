#version 150 core

in vec4 position;
in vec4 noraml;
in vec4 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform samplerBuffer tex_pos;

void main()
{
	vec4 p = view * model * vec4(position.xyz,1.0f);
	gl_Position = proj * p;
}