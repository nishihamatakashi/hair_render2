#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

layout (lines_adjacency) in;
layout(triangle_strip, max_vertices = 48) out;

// 変換行列
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 adjoint;
uniform vec3 lpos;
uniform float knots;
uniform float hlength;

//light情報
uniform vec3 lvector;

//0：Point,1：Spot,2：Area,3：Hemi,4：Sun
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

out vec2 tc;
out vec3 nnn;
out vec3 nv;
out vec3 nl;
out vec3 ntt;
out float h;
out float water;
uniform float weight;
uniform float corners;
uniform float sharp;

vec3 loadRotateSimple(vec3 p, vec3 p1, vec3 Aaxis, float theta)
{
	vec3 tsp = p - p1;

	vec3 pos; 

	pos.x = tsp.x * (cos(theta) + Aaxis.x * Aaxis.x * (1 - cos(theta)))
		+ tsp.y * (Aaxis.x * Aaxis.y * (1 - cos(theta)) - Aaxis.z * sin(theta))
		+ tsp.z * (Aaxis.x * Aaxis.z * (1 - cos(theta)) + Aaxis.y * sin(theta));

	pos.y = tsp.x * (Aaxis.y * Aaxis.x * (1 - cos(theta)) + Aaxis.z * sin(theta))
		+ tsp.y * (cos(theta) + Aaxis.y * Aaxis.y * (1 - cos(theta)))
		+ tsp.z * (Aaxis.y * Aaxis.z * (1 - cos(theta)) - Aaxis.x * sin(theta));

	pos.z = tsp.x * (Aaxis.z * Aaxis.x * (1 - cos(theta)) - Aaxis.y * sin(theta))
		+ tsp.y * (Aaxis.z * Aaxis.y * (1 - cos(theta)) + Aaxis.x * sin(theta))
		+ tsp.z * (cos(theta) + Aaxis.z * Aaxis.z * (1 - cos(theta)));

	pos += p1;

	return pos;
}


void main(){
	float klength = hlength / knots;
	vec4 p1 = gl_in[0].gl_Position;	//ひとつ前の点
	vec4 p0 = gl_in[1].gl_Position;	//現在の点
	vec4 p2 = gl_in[2].gl_Position;	//次の点
	vec4 p3 = gl_in[3].gl_Position;	//2つ次の点

	//接線方向，垂直平面の計算
	vec3 nt1 = normalize(p2.xyz - p1.xyz);		//接線
	vec3 nt2 = normalize(p3.xyz - p0.xyz);		//接線
	vec3 nb1 = normalize(vec3(nt1.z,0.0,-nt1.x));	//従法線
	vec3 nb2 = normalize(vec3(nt2.z,0.0,-nt2.x));	//従法線
	vec3 tp1 = nb1 * (weight * ((knots - 1) - p0.w * sharp) / knots);	//移動先1
	vec3 tp2 = nb2 * (weight * ((knots - 1) - p2.w * sharp) / knots);	//移動先

	//光の計算
	vec4 lv = vec4(lpos,1.0f);

	//回転角度
	float angles = 3.141592 * 2.0f / int(corners);

	//3角形の基本座標1
	vec4 triangle0[] = vec4[](
		vec4(tp1, 0.0),
		vec4(0.0),
		vec4(0.0),
		vec4(0.0)
	);

	//3角形の基本座標2
	vec4 triangle1[] = vec4[](
		vec4(tp2, 0.0),
		vec4(0.0)
	);
	
	//回転の原点
	vec3 o = vec3(0.0,0.0,0.0);

	for(int j = 0; j < int(corners); ++j){
		
		//一つ目の三角形
		vec3 pos0 = loadRotateSimple(triangle0[0].xyz,o,nt1,angles);
		vec3 pos1 = loadRotateSimple(triangle1[0].xyz,o,nt2,angles);
		triangle0[1] = vec4(pos0,0.0f);
		triangle1[1] = vec4(pos1,0.0f);
		triangle0[2] = vec4(triangle1[0].xyz + (p2.xyz - p0.xyz),0.0);
		triangle0[3] = vec4(triangle1[1].xyz + (p2.xyz - p0.xyz),0.0);
		for (int i = 0; i < 4; ++i)
		{
			vec4 out_p = vec4(p0.xyz,1.0f) + triangle0[i];
			vec4 pm = model * out_p;
			vec4 pv = view * model * out_p;
			nv = -normalize(pv.xyz);

			//光の計算
			vec3 lvec = (lv.xyz * 1.0f - pm.xyz * 1.0f).xyz;
			if(ltype == Point){ nl = normalize(lvec);
			}else{ nl = normalize(lvector);}
			
			//hの算出
			vec3 pc = i>2 ? p0.xyz : p2.xyz;
			vec3 p0pm = pc - out_p.xyz;
			vec3 hnl = dot(p0pm,nl)/length(p0pm) * p0pm - nl;
			float sigma = cross(p0pm,hnl).y;
			h = length(hnl) * (step(0.0f, sigma) - 0.5f) * 2.0f;

			//接線の算出
			ntt = i <= 1 ? nt1 : nt2;
			ntt = p0.w == 0.0 ? nt2 : ntt;
			ntt = (adjoint * vec4(ntt,1.0)).xyz;

			//法線の算出
			nnn = i < 2 ? out_p.xyz - p0.xyz.xyz : out_p.xyz - p2.xyz;
			nnn = normalize(nnn);

			//テクスチャ座標の算出
			float y0 = 1.0f - p0.w / knots;
			float y1 = 1.0f - p2.w / knots;
			float x0 = 0.0f;
			float x1 = 1.0f;
			vec2 texture = vec2(0.0);
			texture.x = i % 2 == 0 ? x0 : x1;
			texture.y = i > 1 ? y1 : y0;
			tc = texture;
			
			//水の設定
			water = i <= 1 ?gl_in[1].gl_PointSize : gl_in[2].gl_PointSize;

			gl_Position = proj * pv;
			EmitVertex();
		}
		EndPrimitive();
		triangle0[0] = triangle0[1];
		triangle1[0] = triangle1[1];
	}
}