#version 150 core
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_NV_shadow_samplers_cube: enable

layout(location = 0)out vec4 fragment;

uniform samplerCube cubemap;

in vec3 vtc;

void main()
{
	//�ŏI�I�ȐF���Z�o
	fragment = texture(cubemap,vtc) * 1.1f;
}