#version 150 core
#extension GL_ARB_explicit_attrib_location: enable

layout(location = 0)out vec4 fragment;

uniform mat4 view;
uniform mat4 proj;

//light情報
uniform vec3 lpos;
uniform vec3 lvector;
uniform vec3 lcolor;

uniform vec2 lsize;

uniform float lenergy;
uniform float ldistance;

uniform float lspot_size;
uniform float lblend;
uniform float lgamma;

//0：Point,1：Spot,2：Area,3：Hemi,4：Sun
uniform int ltype;
int Point = 0;
int Spot = 1;
int Area = 2;
int Hemi = 3;
int Sun = 4;

//材質情報
uniform vec3 kdiff;
uniform vec3 kspec;
uniform vec3 kamb;
uniform float kshi;
uniform float beta_n;		//毛の方位角の粗さ
uniform float beta_m;		//毛の仰角の粗さ
uniform float alpha;		//毛の表面のオフセット角度
uniform float sigma;
uniform float transparency;

// テクスチャ座標のサンプラ
uniform sampler2D dmap;		// diffuse map
uniform sampler2D nmap;		// normal map
uniform sampler2D smap;		// specular map
uniform sampler2D depthmap;
uniform samplerCube cubemap;//cube map
uniform mat4 texFlag;

in vec2 tc;
in vec3 nl;
in vec3 nv;
in vec3 ntt;
in float h;

const float PI = 3.141592;
const float eta = 1.55f;		//コルテックス層，メデュラ層の屈折率
vec3 sigma_a = vec3(sigma);	//毛内部の吸収係数

float SafeSqrt(float x) {
	return sqrt(max(float(0.0), x));
}

float SafeAsin(float x) {
	return asin(clamp(x, -1.0, 1.0));
}

//第一種Bessel関数
float I0(float x,int n = 10)
{
	float sum = 0.0f;
	float fact_m = 1.0f;
	float x_over_2_pow_2m = 1.0f;
	float x_over_2 = x * 0.5f;
	for(int m = 0; m < n;++m)
	{
		fact_m *= (m * step(0.5f,float(m)) + 1.0f * step(float(m),0.5f));
		sum += 1.0/(fact_m * fact_m) * x_over_2_pow_2m;
		x_over_2_pow_2m *= x_over_2 * x_over_2;
	}
	return sum;
}

//Eugene d'EonによるI0の補正
float logI0(float x)
{
	float i0 = x + 0.5f * (-log(2.0*PI)+log(1.0/x) + 1.0 / (8.0*x));
	float i1 = log(I0(x));

	//xが12以上のときi0を用いる
	return step(11.5,x) * i0 + step(x,11.5) * i1;
}

//経度拡散関数
float Mp(float sinThetaI,float cosThetaI,float sinThetaO,float cosThetaO,float v)
{
	float a = cosThetaI * cosThetaO / v;
	float b = sinThetaI * sinThetaO / v;

	//vが1以下のとき，Eugene d'Eonによる補正を入れる
	float mp = 
		step(v,0.1) * exp(logI0(a) - b - 1.0 / v + 0.6931 + log(1.0/(2.0 * v)))+
		step(0.1,v) * (exp(-b) * I0(a)) / (sinh(1.0/v) * 2.0f * v);
	return mp;

}

//吸収係数
float FrDielectric(float cosThetaI, float etaI,float etaT)
{
	cosThetaI = clamp(cosThetaI , -1.0,1.0);
	bool entering = cosThetaI > 0.0f;
	if(!entering)
	{
		float tmp = etaI;
		etaI = etaT;
		etaT = tmp;
		cosThetaI = abs(cosThetaI);
	}

	//Compute_cosThetaT_using Shell's Law
	float sinThetaI = sqrt(max(0.0,1.0 - cosThetaI * cosThetaI));
	float sinThetaT = etaI / etaT * sinThetaI;

	//Handle total internal reflection
	if(sinThetaT >= 1.0) return 1;

	float cosThetaT = sqrt(max(0.0,1.0-sinThetaT * sinThetaT));
	float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) / ((etaT * cosThetaI) + (etaI * cosThetaT));
	float Rparp = ((etaI * cosThetaI) - (etaT * cosThetaT)) / ((etaI * cosThetaI) + (etaT * cosThetaT));
	return (Rparl * Rparl + Rparp * Rparp)  * 0.5;
} 

float Phi(int p,float gammaO,float gammaT)
{
	return 2.0 * p * gammaT - 2.0 * gammaO + p * PI;
}

float Logistic(float x, float s) {
	x = abs(x);
	float exp_minus_x_over_s = exp(-x / s);
	return exp_minus_x_over_s / (s * (1.0 + exp_minus_x_over_s) * (1.0 + exp_minus_x_over_s));
}

float LogisticCDF(float x, float s) {
	return 1.0 / (1.0 + exp(-x / s));
}

float TrimmedLogistic(float x, float s, float a, float b) {
	return Logistic(x, s) / (LogisticCDF(b, s) - LogisticCDF(a, s));
}

//方位角方向の拡散
float Np(float phi,int p,float s,float gammaO,float gammaT)
{
	float dphi = phi - Phi(p,gammaO,gammaT);

	while(dphi > PI) dphi -= 2.0 * PI;
	while(dphi < -PI) dphi +=2.0 * PI;

	return TrimmedLogistic(dphi,s,-PI,PI);
}

//vとbetamの変換
vec3 beta_m_to_v(float bm)
{
	vec3 vs;
	float x = 0.726 * bm + 0.812 * bm * bm + 3.7 * pow(bm,20);
	vs.x = x * x;
	vs.y = 0.25 * vs.x;
	vs.z = 4.0 * vs.y;
	return vs;
}

//sとbetanの変換
float beta_n_to_s(float beta_n)
{
	float SqrtPiOver8 = 0.626657069;
	return SqrtPiOver8 * (0.265 * beta_n * 1.194 * beta_n * beta_n + 5.372 * pow(beta_n,22));
}

float AbsCosThetaForHair(vec3 w)
{
	return sqrt(w.z * w.z + w.y * w.y);
}

//bsdf(wi：光の方向，wo：カメラの方向，h：)
vec3 bsdf(vec3 wi,vec3 wo,float h)
{

	float sinThetaO = wo.x;
	float cosThetaO = SafeSqrt(1.0f - (sinThetaO * sinThetaO));
	float phiO = atan(wo.z,wo.y);
	
	float sinThetaI = wi.x;
	float cosThetaI = SafeSqrt(1.0f - (sinThetaI * sinThetaI));
	float phiI = atan(wi.z,wi.y);
	
	float s = beta_n_to_s(beta_n);

	float sinThetaT = sinThetaO / eta;
	float cosThetaT = SafeSqrt(1.0f - (sinThetaT * sinThetaT));

	float eta_p = sqrt(eta * eta - (sinThetaO * sinThetaO)) / cosThetaO;
	float sinGammaT = h / eta_p;
	float cosGammaT = SafeSqrt(1 - (sinGammaT * sinGammaT));
	float gammaT = SafeAsin(sinGammaT);

	float l = 2.0f * cosGammaT / cosThetaT;

	vec3 T = exp(-sigma_a * l);

	vec3 sin2kAlpha;
	vec3 cos2kAlpha;
	sin2kAlpha.x = sin(alpha);
	cos2kAlpha.x = SafeSqrt(1 - (sin2kAlpha.x * sin2kAlpha.x));
	
	sin2kAlpha.y = 2 * cos2kAlpha.x * sin2kAlpha.x;
	cos2kAlpha.y = (cos2kAlpha.x * cos2kAlpha.x) - (sin2kAlpha.x * sin2kAlpha.x);

	sin2kAlpha.z = 2 * cos2kAlpha.y * sin2kAlpha.y;
	cos2kAlpha.z = (cos2kAlpha.y * cos2kAlpha.y) - (sin2kAlpha.y * sin2kAlpha.y);

	float gammaO = SafeAsin(h);

	float phi = phiI - phiO;

	float cosGammaO = SafeSqrt(1.0 - h * h);
	float cosThetaA = cosThetaO * cosGammaO;
	float f = FrDielectric(cosThetaA,1.0,eta);
	vec3 ap0 = vec3(f);
	vec3 ap1 = (1-f)*(1-f) * T;
	vec3 ap2 = ap1 * f *T /(vec3(1.0) - T * f);

	vec3 v = beta_m_to_v(beta_m);

	vec3 fsum = vec3(0.0);

	float sinThetaIp,cosThetaIp;

	
	sinThetaIp = sinThetaI * cos2kAlpha.y + cosThetaI * sin2kAlpha.y;
	cosThetaIp = cosThetaI * cos2kAlpha.y - sinThetaI * sin2kAlpha.y;
	cosThetaIp = abs(cosThetaIp);
	fsum += Mp(sinThetaIp,cosThetaIp,sinThetaO,cosThetaO,v.x) * ap0 * Np(phi,0,s,gammaO,gammaT);
	
	
	sinThetaIp = sinThetaI * cos2kAlpha.x - cosThetaI * sin2kAlpha.x;
	cosThetaIp = cosThetaI * cos2kAlpha.x + sinThetaI * sin2kAlpha.x;
	cosThetaIp = abs(cosThetaIp);
	fsum += Mp(sinThetaIp,cosThetaIp,sinThetaO,cosThetaO,v.y) * ap1 * Np(phi,1,s,gammaO,gammaT);
	
	sinThetaIp = sinThetaI * cos2kAlpha.z - cosThetaI * sin2kAlpha.z;
	cosThetaIp = cosThetaI * cos2kAlpha.z + sinThetaI * sin2kAlpha.z;
	cosThetaIp = abs(cosThetaIp);
	fsum += Mp(sinThetaIp,cosThetaIp,sinThetaO,cosThetaO,v.y) * ap1 * Np(phi,1,s,gammaO,gammaT);
	fsum += Mp(sinThetaIp,cosThetaIp,sinThetaO,cosThetaO,v.z) * ap2 / (2.0f * PI);
	
	
	if (cosThetaI > 0) {
		fsum /= sqrt(wi.z * wi.z + wi.y * wi.y);
	}
	
	return fsum;
}

void main()
{

	vec3 lc = lcolor;
	vec3 nbb = vec3(-ntt.z,0.0,ntt.x);
	vec3 nnn = cross(ntt,nbb);
	mat3 Tan = transpose(mat3(ntt,nbb,nnn));
	
	vec3 nnl = (Tan * nl);
	vec3 nnv = (Tan * nv);

	vec4 Dcolor = vec4(kdiff,1.0);
	if(texFlag[0][0] == 1){ Dcolor = texture(dmap,tc); }

	float a = Dcolor.a * transparency;

	//拡散反射光を計算
	float lt = dot(nl, ntt);
	float lt2 = sqrt(1.0 - lt * lt);

	vec3 Idiff = lt2 * lc * Dcolor.rgb;

	//鏡面反射光を計算
	vec3 Ispec =  bsdf(nnl,nnv,h) * kspec;
	
	//環境光を計算
	vec3 Iamb = kamb * lc;

	//最終的な色を算出
	vec4 output = vec4(Ispec +Idiff + Iamb,a);
	fragment = output;
}