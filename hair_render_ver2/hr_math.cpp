#include"hr_math.h"

//要素nから2^nのサイズを得る
void calcResolution(const int n, int *size)
{
	int cell = int(ceil(log2(n)));
	cell = cell % 2 == 1 ? cell + 1 : cell;
	int tex_size = int(pow(2, cell));
	int cell_size = int(sqrt(tex_size));
	size[0] = size[1] = cell_size;
}

//マウスの(2D)移動からオブジェクトの移動方向を算出する
void mouseTranslate(float x, float y, float *dir, float *p)
{
	float d = distance(dir[0], dir[1], dir[2]);
	Normalize(dir);

	float xz_ver[3];
	xz_ver[0] = -dir[2];
	xz_ver[1] = 0.0f;
	xz_ver[2] = dir[0];
	Normalize(xz_ver);

	float par = -4.0f;
	x *= par;
	y *= par;

	p[0] = -xz_ver[0] * x * d;
	p[1] = -y * d;
	p[2] = -xz_ver[2] * x* d;
}

//マウスの(2D)移動から回転を算出する
void mouseRotate(float x, float y, float *dir, float *p0, float *p1)
{
	float d = distance(dir[0], dir[1], dir[2]);
	Normalize(dir);

	float Y_AXIS[3] = { 0.0,1.0,0.0 };
	float N_AXIS[3] = { dir[2],0.0f,-dir[0] };

	float theta = x * 3.141512f;
	float phay = y * 3.141512f;

	loadRotateSimple(p0, p1, Y_AXIS, theta);
	loadRotateSimple(p0, p1, N_AXIS, phay);
}

//マウスのホイール移動から移動方向を算出する
void mouseZoom(float w, float *dir, float *p)
{
	float d = distance(dir[0], dir[1], dir[2]);
	Normalize(dir);


	p[0] += dir[0] * w * d;
	p[1] += dir[1] * w * d;
	p[2] += dir[2] * w * d;
}

//空間登録するハッシュ関数
int setSpace_Hash(float x, float y, float z, int cell_num, float length)
{
	//空間の端の座標
	float f[3] = { -length,-length ,-length };
	float fm[3] = { length ,length ,length };

	//格子の一片の長さ
	float cl_x, cl_y, cl_z;

	//格子座標
	int cp_x, cp_y, cp_z;

	cl_x = fabs(fm[0] - f[0]) / float(cell_num);
	cl_y = fabs(fm[1] - f[1]) / float(cell_num);
	cl_z = fabs(fm[2] - f[2]) / float(cell_num);

	cp_x = int(abs(x - f[0]) / cl_x);
	cp_y = int(abs(y - f[1]) / cl_y);
	cp_z = int(abs(z - f[2]) / cl_z);

	//出力するID情報
	int out_ID = cp_x * cell_num * cell_num + cp_y * cell_num + cp_z;

	if (abs(x) > length || abs(y) > length || abs(z) > length)
	{
		return NULL;
	}

	return out_ID;
}

//距離関数
float distance(float x, float y, float z) {
	return sqrt(x*x + y * y + z * z);
}

//正規化関数
void Normalize(float *p) {
	float d = distance(p[0], p[1], p[2]);
	p[0] /= d;
	p[1] /= d;
	p[2] /= d;
}

//行列同士の積
float *multiply(const float *m0, const float *m1, float *m)
{
	for (int ji = 0; ji < 16; ++ji) {
		const int i(ji & 3), j(ji&~3);
		m[ji] = m0[i] * m1[j] + m0[4 + i] * m1[j + 1] + m0[8 + i] * m1[j + 2] + m0[12 + i] * m1[j + 3];
	}
	return m;
}

//随伴行列
float *adjoint(float *m0, float *m1) {
	m1[0] = m0[5] * m0[10] - m0[6] * m0[9];
	m1[1] = m0[6] * m0[8] - m0[4] * m0[10];
	m1[2] = m0[4] * m0[9] - m0[5] * m0[8];

	m1[4] = m0[9] * m0[2] - m0[10] * m0[1];
	m1[5] = m0[10] * m0[0] - m0[8] * m0[2];
	m1[6] = m0[8] * m0[1] - m0[9] * m0[0];

	m1[8] = m0[1] * m0[6] - m0[2] * m0[5];
	m1[9] = m0[2] * m0[4] - m0[0] * m0[6];
	m1[10] = m0[0] * m0[5] - m0[1] * m0[4];

	m1[3] = m1[7] = m1[11] = m1[12] = m1[13] = m1[14] = 0.0f;
	m1[15] = 1.0f;

	return m1;
}

//球面線形補間
float *slerp(float *q1, float *q2, float *q3, float t) {
	float qr = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];
	float ss = 1.0f - qr * qr;
	float sp;
	if (ss < 0.0 || (sp = sqrt(ss)) == 0.0) {
		q3[0] = q1[0];
		q3[1] = q1[1];
		q3[2] = q1[2];
		q3[3] = q1[3];
	}
	else {
		float ph = acos(qr);
		float pt = ph * t;
		float t1 = sin(pt) / sp;
		float t0 = sin(ph - pt) / sp;
		q3[0] = q1[0] * t0 + q2[0] * t1;
		q3[1] = q1[1] * t0 + q2[1] * t1;
		q3[2] = q1[2] * t0 + q2[2] * t1;
		q3[3] = q1[3] * t0 + q2[3] * t1;
	}
	return q3;
}


//クォータニオンの生成
float *qmake(float *n, float a, float *q) {
	q[0] = n[0] * sin(a / 2);
	q[1] = n[1] * sin(a / 2);
	q[2] = n[2] * sin(a / 2);
	q[3] = cos(a / 2);

	return q;
}

//クォータニオン回転
float *loadQuaternion(float *q, float *m) {
	m[0] = 1 - 2 * q[1] * q[1] - 2 * q[2] * q[2];
	m[4] = 2 * q[0] * q[1] + 2 * q[3] * q[2];
	m[8] = 2 * q[0] * q[2] - 2 * q[3] * q[1];
	m[12] = 0;

	m[1] = 2 * q[0] * q[1] - 2 * q[3] * q[2];
	m[5] = 1 - 2 * q[0] * q[0] - 2 * q[2] * q[2];
	m[9] = 2 * q[1] * q[2] + 2 * q[3] * q[0];
	m[13] = 0;

	m[2] = 2 * q[0] * q[2] + 2 * q[3] * q[1];
	m[6] = 2 * q[1] * q[2] - 2 * q[3] * q[0];
	m[10] = 1 - 2 * q[0] * q[0] - 2 * q[1] * q[1];
	m[14] = 0;

	m[3] = 0;
	m[7] = 0;
	m[11] = 0;
	m[15] = 1;

	return m;
}

//任意軸回転(変換行列を求めずそのまま)
void loadRotateSimple(float *p, float *p1, float *Aaxis, float theta)
{
	float tsp[3] = { p[0] - p1[0], p[1] - p1[1], p[2] - p1[2] };

	p[0] = tsp[0] * (cos(theta) + Aaxis[0] * Aaxis[0] * (1 - cos(theta)))
		+ tsp[1] * (Aaxis[0] * Aaxis[1] * (1 - cos(theta)) - Aaxis[2] * sin(theta))
		+ tsp[2] * (Aaxis[0] * Aaxis[2] * (1 - cos(theta)) + Aaxis[1] * sin(theta));

	p[1] = tsp[0] * (Aaxis[1] * Aaxis[0] * (1 - cos(theta)) + Aaxis[2] * sin(theta))
		+ tsp[1] * (cos(theta) + Aaxis[1] * Aaxis[1] * (1 - cos(theta)))
		+ tsp[2] * (Aaxis[1] * Aaxis[2] * (1 - cos(theta)) - Aaxis[0] * sin(theta));

	p[2] = tsp[0] * (Aaxis[2] * Aaxis[0] * (1 - cos(theta)) - Aaxis[1] * sin(theta))
		+ tsp[1] * (Aaxis[2] * Aaxis[1] * (1 - cos(theta)) + Aaxis[0] * sin(theta))
		+ tsp[2] * (cos(theta) + Aaxis[2] * Aaxis[2] * (1 - cos(theta)));

	p[0] += p1[0];
	p[1] += p1[1];
	p[2] += p1[2];
}

//任意軸回転
float *loadRotate(float a, float *n, float *m)
{
	float x(n[0]), y(n[1]), z(n[2]);
	float x2(x*x), y2(y*y), z2(z*z);
	float xy(x*y), yz(y*z), zx(z*x);
	float c0(cos(a)), c1(1 - c0), s(sin(a));

	m[0] = (1.0f - x2) * c0 + x2;
	m[1] = xy * c1 + z * s;
	m[2] = zx * c1 - y * s;

	m[4] = xy * c1 - z * s;
	m[5] = (1.0f - y2) * c0 + y2;
	m[6] = yz * c1 + x * s;

	m[8] = zx * c1 + y * s;
	m[9] = yz * c1 - x * s;
	m[10] = (1.0f - z2) * c0 + z2;

	m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;

	return m;
}

//オイラー角回転
float *loadEulerRotate(const float *r, float *m)
{
	loadIdentity(m);
	float XAXIS[3] = { 1.0f,0.0f,0.0f };
	float YAXIS[3] = { 0.0f,1.0f,0.0f };
	float ZAXIS[3] = { 0.0f,0.0f,1.0f };

	float X_theta = r[0] / 180.0f * 3.1415926f;
	float Y_theta = r[1] / 180.0f * 3.1415926f;
	float Z_theta = r[2] / 180.0f * 3.1415926f;

	
	Rotate(Y_theta, YAXIS, m);
	Rotate(X_theta, XAXIS, m);
	Rotate(Z_theta, ZAXIS, m);
	
	return m;
}

//平行移動関数
float *loadTranslate(float x, float y, float z, float *m)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;
	m[1] = m[2] = m[4] = m[6] = m[8] = m[9] = m[3] = m[7] = m[11] = 0.0f;
	m[0] = m[5] = m[10] = 1.0f;
	m[15] = 1.0f;

	return m;
}

//拡大縮小関数
float *loadScale(float x, float y, float z, float *m)
{
	m[0] = x;
	m[5] = y;
	m[10] = z;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;

	return m;
}

//ビュー変換行列
float *loadLookat(const float *e, const float *g, const float *u, float *m) {

	//平行移動の変換行列
	float tv[16];
	distance(-e[0], -e[1], -e[2]);
	loadTranslate(-e[0], -e[1], -e[2], tv);

	//t軸 = e - g
	const float t[3] = {
		(e[0] - g[0]),
		(e[1] - g[1]),
		(e[2] - g[2])
	};

	//r軸 = u × t軸
	const float r[3] = {
		(u[1] * t[2] - u[2] * t[1]),
		(u[2] * t[0] - u[0] * t[2]),
		(u[0] * t[1] - u[1] * t[0])
	};

	//s軸 = t軸 × r軸
	const float s[3] = {
		(t[1] * r[2] - t[2] * r[1]),
		(t[2] * r[0] - t[0] * r[2]),
		(t[0] * r[1] - t[1] * r[0])
	};

	//s軸の長さをチェック
	const float s2(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
	if (s2 == 0.0f)return m;

	//回転行列の変換
	float rv[16];

	//r軸を正規化し，配列変数に格納
	const float rl(sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]));
	rv[0] = r[0] / rl;
	rv[4] = r[1] / rl;
	rv[8] = r[2] / rl;

	//s軸を正規化して配列変数に格納
	const float sl = sqrt(s2);
	rv[1] = s[0] / sl;
	rv[5] = s[1] / sl;
	rv[9] = s[2] / sl;

	//s軸を正規化して配列変数に格納
	const float tl = sqrt(t[0] * t[0] + t[1] * t[1] + t[2] * t[2]);
	rv[2] = t[0] / tl;
	rv[6] = t[1] / tl;
	rv[10] = t[2] / tl;

	// 残りの成分   
	rv[3] = rv[7] = rv[11] = rv[12] = rv[13] = rv[14] = 0.0f;
	rv[15] = 1.0f;

	// 視点の平行移動の変換行列に視線の回転の変換行列を乗じる
	return multiply(rv, tv, m);
}

//ビュー変換行列
float *loadSkyLookat(const float *e,const float *g,const float *u, float *rv) {

	//t軸 = e - g
	const float t[3] = {
		(e[0] - g[0]),
		(e[1] - g[1]),
		(e[2] - g[2])
	};

	//r軸 = u × t軸
	const float r[3] = {
		(u[1] * t[2] - u[2] * t[1]),
		(u[2] * t[0] - u[0] * t[2]),
		(u[0] * t[1] - u[1] * t[0])
	};

	//s軸 = t軸 × r軸
	const float s[3] = {
		(t[1] * r[2] - t[2] * r[1]),
		(t[2] * r[0] - t[0] * r[2]),
		(t[0] * r[1] - t[1] * r[0])
	};

	//s軸の長さをチェック
	const float s2(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
	if (s2 == 0.0f)return rv;

	//r軸を正規化し，配列変数に格納
	const float rl(sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]));
	rv[0] = r[0] / rl;
	rv[4] = r[1] / rl;
	rv[8] = r[2] / rl;

	//s軸を正規化して配列変数に格納
	const float sl = sqrt(s2);
	rv[1] = s[0] / sl;
	rv[5] = s[1] / sl;
	rv[9] = s[2] / sl;

	//s軸を正規化して配列変数に格納
	const float tl = sqrt(t[0] * t[0] + t[1] * t[1] + t[2] * t[2]);
	rv[2] = t[0] / tl;
	rv[6] = t[1] / tl;
	rv[10] = t[2] / tl;

	// 残りの成分   
	rv[3] = rv[7] = rv[11] = rv[12] = rv[13] = rv[14] = 0.0f;
	rv[15] = 1.0f;

	// 視点の平行移動の変換行列に視線の回転の変換行列を乗じる
	return rv;
}

// 直交投影変換行列を設定する
float *loadOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar, float *m)
{
	const float dx(right - left);
	const float dy(top - bottom);
	const float dz(zFar - zNear);

	if (dx != 0.0f && dy != 0.0f&&dz != 0.0f)
	{
		m[0] = 2.0f / dx;
		m[5] = 2.0f / dy;
		m[10] = -2.0f / dz;
		m[12] = -(right + left) / dx;
		m[13] = -(top + bottom) / dy;
		m[14] = -(zFar + zNear) / dz;
		m[15] = 1.0f;
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = 0.0f;
	}

	return m;
}

// 透視投影変換行列を m に設定する 
float *loadFrustum(float left, float right, float bottom, float top, float zNear, float zFar, float *m)
{
	const float dx(right - left);
	const float dy(top - bottom);
	const float dz(zFar - zNear);

	if (dx != 0.0f && dy != 0.0f&&dz != 0.0f)
	{
		m[0] = 2.0f  * zNear / dx;
		m[5] = 2.0f * zNear / dy;
		m[8] = (right + left) / dx;
		m[9] = (top + bottom) / dy;
		m[10] = -(zFar + zNear) / dz;
		m[11] = -1.0f;
		m[14] = -2.0f * zFar * zNear / dz;
		m[15] = 1.0f;
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[12] = m[13] = m[15] = 0.0f;
	}

	return m;
}

float *loadPerspective(float fovy, float aspect, float zNear, float zFar, float *m)
{
	const float dz = zFar - zNear;
	const float F = 1.0f / tan(fovy * 0.5f);
	if (dz != 0.0f) {
		m[0] = F / aspect;
		m[5] = F;
		m[10] = -(zFar + zNear) / dz;
		m[11] = -1.0f;
		m[14] = -2.0f * zFar * zNear / dz;
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[12] = m[13] = m[15] = 0.0f;
	}
	return m;
}

float *loadIdentity(float *m)
{
	return loadTranslate(0.0f, 0.0f, 0.0f, m);
}

float *Translate(float x, float y, float z, float *m)
{
	float m0[16], m1[16];
	std::copy(m, m + 16, m1);
	return multiply(loadTranslate(x, y, z, m0), m1, m);
}

float *EulerRotate(const float *r, float *m)
{
	float m0[16], m1[16];
	std::copy(m, m + 16, m1);
	return multiply(loadEulerRotate(r, m0), m1, m);
}

float *Rotate(float a, float *n, float *m)
{
	float m0[16], m1[16];
	std::copy(m, m + 16, m1);
	return multiply(loadRotate(a, n, m0), m1, m);
}

float *Scale(float x, float y, float z, float *m)
{
	float m0[16], m1[16];
	std::copy(m, m + 16, m1);
	return multiply(loadScale(x, y, z, m0), m1, m);
}

float *Lookat(float *e, float *g, float *u, float *m)
{
	float m0[16], m1[16];
	std::copy(m, m + 16, m1);
	return multiply(loadLookat(e, g, u, m0), m1, m);
}

float *Quaternion(float *q, float *m)
{
	float m0[16], m1[16];
	std::copy(m, m + 16, m1);
	return multiply(m1, loadQuaternion(q, m0), m);
}

float *Inverse(float *m0, float *m1)
{
	float lu[20], *plu[4];

	// j 行の要素の値の絶対値の最大値を plu[j][4] に求める
	for (int j = 0; j < 4; ++j)
	{
		float max(fabs(*(plu[j] = lu + 5 * j) = *(m0++)));

		for (int i = 0; ++i < 4;)
		{
			float a(fabs(plu[j][i] = *(m0++)));
			if (a > max) max = a;
		}
		if (max == 0.0f) return m0;
		plu[j][4] = 1.0f / max;
	}

	// ピボットを考慮した LU 分解
	for (int j = 0; j < 4; ++j)
	{
		float max(fabs(plu[j][j] * plu[j][4]));
		int i = j;

		for (int k = j; ++k < 4;)
		{
			float a(fabs(plu[k][j] * plu[k][4]));
			if (a > max)
			{
				max = a;
				i = k;
			}
		}
		if (i > j)
		{
			float *t(plu[j]);
			plu[j] = plu[i];
			plu[i] = t;
		}
		if (plu[j][j] == 0.0f) return m0;
		for (int k = j; ++k < 4;)
		{
			plu[k][j] /= plu[j][j];
			for (int i = j; ++i < 4;)
			{
				plu[k][i] -= plu[j][i] * plu[k][j];
			}
		}
	}

	// LU 分解から逆行列を求める
	for (int k = 0; k < 4; ++k)
	{
		// array に単位行列を設定する
		for (int i = 0; i < 4; ++i)
		{
			m1[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0f : 0.0f;
		}
		// lu から逆行列を求める
		for (int i = 0; i < 4; ++i)
		{
			for (int j = i; ++j < 4;)
			{
				m1[j * 4 + k] -= m1[i * 4 + k] * plu[j][i];
			}
		}
		for (int i = 4; --i >= 0;)
		{
			for (int j = i; ++j < 4;)
			{
				m1[i * 4 + k] -= plu[i][j] * m1[j * 4 + k];
			}
			m1[i * 4 + k] /= plu[i][i];
		}
	}
	return m1;
}

float *CalcModelMatrix(float *t, float *r, float *s, float *m) {
	Translate(t[0], t[1], t[2], m);
	EulerRotate(r, m);
	Scale(s[0], s[1], s[2], m);
	return m;
}