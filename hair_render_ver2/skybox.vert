#version 150 core
#extension GL_ARB_explicit_attrib_location: enable 

layout(location = 0) in vec4 position;	//頂点座標

uniform mat4 view;		//view行列
uniform mat4 proj;		//projection行列
uniform mat4 adjoint;
out vec3 vtc;

void main()
{
	vtc =  -(position).xyz;
	gl_Position = proj * view * position;
}