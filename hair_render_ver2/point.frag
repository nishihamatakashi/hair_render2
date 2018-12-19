#version 150 core

out vec4 fragment;

uniform vec3 color;

void main()
{
	fragment = vec4(color,1.0f);
}