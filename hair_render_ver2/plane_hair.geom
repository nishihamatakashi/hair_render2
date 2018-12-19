#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

layout (lines_adjacency) in;
layout(triangle_strip, max_vertices = 6) out;

// ïœä∑çsóÒ
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lpos;
uniform float knots;

out vec2 tc;
out vec4 pm;
out vec4 pv;
out vec4 lv;
out vec3 nt;

float mesh_length = 0.01f;

void main(){
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	
	nt = normalize(p2.xyz - p0.xyz);
	vec3 nl = normalize(p2.xyz - p1.xyz);
	vec3 nb = normalize(vec3(nl.z,0.0,-nl.x));
	vec3 tp = nb * mesh_length;

	lv = view * vec4(lpos,1.0f);

	vec4 triangle0[] = vec4[](
		vec4(tp,0.0),
		vec4(-tp,0.0),
		vec4(tp + (p2.xyz - p1.xyz),0.0)
	);

	vec4 triangle1[] = vec4[](
		vec4(tp,0.0),
		vec4(-tp,0.0),
		vec4(-tp - (p2.xyz - p1.xyz),0.0)
		);

	float tc_move0 = (knots - p1.w) / knots;
	float tc_move1 = (knots - p2.w) / knots;

	vec2 texcoord0[] = vec2[]
	(
		vec2(0,tc_move0),
		vec2(1,tc_move0),
		vec2(0,tc_move1)
	);
	
	vec2 texcoord1[] = vec2[]
	(
		vec2(0,tc_move1),
		vec2(1,tc_move1),
		vec2(1,tc_move0)
	);

	for (int i = 0; i < 3; ++i)
	{
		tc = texcoord0[i];
		pm = model * (vec4(p1.xyz,1.0f) + triangle0[i]);
		pv = view * pm;
		gl_Position = proj * pv;
		EmitVertex();
	}
	EndPrimitive();

	for(int i = 0; i <3 ; i++)
	{
		tc = texcoord1[i];
		pm = model * (vec4(p2.xyz,1.0f) + triangle1[i]);
		pv = view * pm;
		gl_Position = proj * pv;
		EmitVertex();
	}
	EndPrimitive();
}