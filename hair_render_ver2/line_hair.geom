#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

layout (lines_adjacency) in;
layout(line_strip, max_vertices = 48) out;

// �ϊ��s��
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lpos;
uniform float knots;

//light���
uniform vec3 lvector;

//0�FPoint,1�FSpot,2�FArea,3�FHemi,4�FSun
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

out vec2 tc;
out vec3 nv;
out vec3 nl;
out vec3 ntt;
out float h;

uniform float weight;
float range = weight;
float hairs = 10.0f;

void main(){
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	
	ntt = normalize(p2.xyz - p0.xyz);
	vec3 nl = normalize(p2.xyz - p1.xyz);
	vec3 nb = normalize(vec3(nl.z,0.0,-nl.x));
	float tp = (2 * range) / hairs;

	//���̌v�Z
	vec4 lv = vec4(lpos,1.0f);

	vec4 lines[] = vec4[](
		vec4(p1.xyz,0.0),
		vec4(p2.xyz,0.0)
	);

	for(int j = 0; j < int(hairs); j++){
		lines[0] = vec4(nb * ((j * tp) - range),0.0);
		lines[1] = vec4(nb * ((j * tp) - range),0.0) + vec4(p2.xyz - p0.xyz,0.0f);
		for (int i = 0; i < 2; ++i)
		{
			vec4 out_p = (vec4(p0.xyz,1.0f) + lines[i]);
			vec4 pm = model * out_p;
			vec4 pv = view * pm;
			nv = -normalize(pv.xyz);

			//���̌v�Z
			vec3 lvec = (lv * 1.0f - pm * 1.0f).xyz;
			if(ltype == Point){ nl = normalize(lvec);
			}else{ nl = normalize(lvector);}
			
			//h�̎Z�o
			vec3 p0pm = p0.xyz - out_p.xyz;
			vec3 hnl = dot(p0pm,nl)/length(p0pm) * p0pm - nl;
			float sigma = cross(p0pm,hnl).y;
			h = length(hnl) * (step(0.0f, sigma) - 0.5f) * 2.0f;
			gl_Position = proj * pv;
			EmitVertex();
		}
		EndPrimitive();
	}
}