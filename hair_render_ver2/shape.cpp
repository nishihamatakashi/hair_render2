#include"shape.h"

Shape::Shape(BASIC3D_TYPE t) 
{
	switch (t) {
	case Cube:
		this->createCube();
		break;
	case Sphere:
		this->createSphere(64, 64);
		break;
	case Skybox:
		this->createSkyboxCube();
		break;
	case Plane:
		this->createPlane();
		break;
	case Billboad:
		this->createBillboad();
		break;
	}
}

Shape::Shape(BASIC3D_TYPE t, float hairs, float knots, float length, float radius, std::vector<float> &first)
{
	switch (t) {
	case SphereHair:
		this->createSphereHair(hairs, knots, length, radius, first);
		break;
	case PlaneHair:
		this->createPlaneHair(hairs, knots, length, radius, first);
		break;
	}
}

void Shape::createPlane() {
	this->vertices = 4;
	this->indices = 2;

	//動的配列の確保
	this->position.resize(this->vertices * 3);
	this->normal.resize(this->vertices * 3);
	this->texcoord.resize(this->vertices * 2);
	this->index.resize(this->indices * 3);;

	//頂点の座標，法線，uvを求める
	this->position.at(0) = -1.0f;
	this->position.at(1) = 0.0f;
	this->position.at(2) = 1.0f;

	this->position.at(3) = -1.0f;
	this->position.at(4) = 0.0f;
	this->position.at(5) = -1.0f;

	this->position.at(6) = 1.0f;
	this->position.at(7) = 0.0f;
	this->position.at(8) = -1.0f;

	this->position.at(9) = 1.0f;
	this->position.at(10) = 0.0f;
	this->position.at(11) = 1.0f;

	this->texcoord.at(0) = 0.0f;
	this->texcoord.at(1) = 1.0f;

	this->texcoord.at(2) = 0.0f;
	this->texcoord.at(3) = 0.0f;

	this->texcoord.at(4) = 1.0f;
	this->texcoord.at(5) = 0.0f;

	this->texcoord.at(6) = 1.0f;
	this->texcoord.at(7) = 1.0f;

	for (int i = 0; i < 4; i++) {
		this->normal.at(i * 3) = 1.0f;
		this->normal.at(i * 3 + 1) = 1.0f;
		this->normal.at(i * 3 + 2) = 0.0f;
	}

	//インデックスを求める
	this->index.at(0) = 0;
	this->index.at(1) = 2;
	this->index.at(2) = 1;

	this->index.at(3) = 0;
	this->index.at(4) = 3;
	this->index.at(5) = 2;
}

void Shape::createBillboad() {
	this->vertices = 4;
	this->indices = 2;

	//動的配列の確保
	this->position.resize(this->vertices * 3);
	this->normal.resize(this->vertices * 3);
	this->texcoord.resize(this->vertices * 2);
	this->index.resize(this->indices * 3);;

	//頂点の座標，法線，uvを求める
	this->position.at(0) = -1.0f;
	this->position.at(1) = 1.0f;
	this->position.at(2) = 0.0f;

	this->position.at(3) = -1.0f;
	this->position.at(4) = -1.0f;
	this->position.at(5) = 0.0f;

	this->position.at(6) = 1.0f;
	this->position.at(7) = -1.0f;
	this->position.at(8) = 0.0f;

	this->position.at(9) = 1.0f;
	this->position.at(10) = 1.0f;
	this->position.at(11) = 0.0f;

	this->texcoord.at(0) = 0.0f;
	this->texcoord.at(1) = 1.0f;

	this->texcoord.at(2) = 0.0f;
	this->texcoord.at(3) = 0.0f;

	this->texcoord.at(4) = 1.0f;
	this->texcoord.at(5) = 0.0f;

	this->texcoord.at(6) = 1.0f;
	this->texcoord.at(7) = 1.0f;

	for (int i = 0; i < 4; i++) {
		this->normal.at(i * 3) = 0.0f;
		this->normal.at(i * 3 + 1) = 0.0f;
		this->normal.at(i * 3 + 2) = 1.0f;
	}

	//インデックスを求める
	this->index.at(0) = 0;
	this->index.at(1) = 2;
	this->index.at(2) = 1;

	this->index.at(3) = 0;
	this->index.at(4) = 3;
	this->index.at(5) = 2;
}

void Shape::createCube() {

	this->vertices = 8;
	this->indices = 12;

	//動的配列の確保
	this->position.resize(this->vertices * 3);
	this->normal.resize(this->vertices * 3);
	this->texcoord.resize(this->vertices * 3);
	this->index.resize(this->indices * 3);

	float length = 1.0f;

	// 頂点の位置データ   
	float pv[8][3] =
	{
		{ length, length, length },  // (0)     
	{ length, -length,  length },  // (1)     
	{ length,  -length,  -length },  // (2)     
	{ length,  length, -length },  // (3)     
	{ -length,  length, length },  // (4)     
	{ -length, -length, length },  // (5)     
	{ -length, -length,  -length },  // (6)     
	{ -length,  length,  -length }   // (7)   
	};

	unsigned int iv[12][3] =
	{
		{ 0,1,2 },  // (0)     
	{ 0,2,3 },  // (1)     
	{ 3,2,6 },  // (2)     
	{ 3,6,7 },  // (3)     
	{ 7,6,5 },  // (4)     
	{ 7,5,4 },  // (5)     
	{ 4,5,1 },  // (6)     
	{ 4,1,0 },  // (7)
	{ 0,3,7 },
	{ 0,7,4 },
	{ 1,5,6 },
	{ 1,6,2 }
	};

	for (int i = 0; i < 8; i++) {

		this->position.at(i * 3) = pv[i][0];
		this->position.at(i * 3 + 1) = pv[i][1];
		this->position.at(i * 3 + 2) = pv[i][2];

		float dir[3] = { pv[i][0],pv[i][1],pv[i][2] };
		Normalize(dir);
		this->normal.at(i * 3) = dir[0];
		this->normal.at(i * 3 + 1) = dir[1];
		this->normal.at(i * 3 + 2) = dir[2];

		texcoord.at(i * 3) = 0.0f;
		texcoord.at(i * 3 + 1) = 0.0f;
	}

	for (int i = 0; i < 12; i++)
	{
		//インデックスを求める
		this->index.at(i * 3) = iv[i][0];
		this->index.at(i * 3 + 1) = iv[i][1];
		this->index.at(i * 3 + 2) = iv[i][2];
	}
}

// 球のデータの作成
void Shape::createSphere(int slices, int stacks)
{
	float radius = 1.0f;

	this->vertices = (stacks + 1) * (slices + 1);
	this->indices = stacks * slices * 2;

	//動的配列の確保
	this->position.resize(this->vertices * 3);
	this->normal.resize(this->vertices * 3);
	this->texcoord.resize(this->vertices * 2);
	this->index.resize(this->indices * 3);

	// 頂点の位置とテクスチャ座標を求める
	for (int k = 0, j = 0; j <= stacks; ++j)
	{
		const float t(static_cast<float>(j) / static_cast<float>(stacks));
		const float ph(3.141593f * t);
		const float y(cos(ph));
		const float r(sin(ph));

		for (int i = 0; i <= slices; ++i)
		{
			const float s(static_cast<float>(i) / static_cast<float>(slices));
			const float th(-2.0f * 3.141593f * s);
			const float x(r * cos(th));
			const float z(r * sin(th));

			// 頂点の座標値
			position.at(k * 3) = x * radius;
			position.at(k * 3 + 1) = y * radius;
			position.at(k * 3 + 2) = z * radius;

			// 頂点の法線ベクトル
			normal.at(k * 3) = x;
			normal.at(k * 3 + 1) = y;
			normal.at(k * 3 + 2) = z;

			// 頂点のテクスチャ座標値
			texcoord.at(k * 2) = s;
			texcoord.at(k * 2 + 1) = t;

			++k;
		}
	}

	// 面の指標を求める
	for (int k = 0, j = 0; j < stacks; ++j)
	{
		for (int i = 0; i < slices; ++i)
		{
			const int count((slices + 1) * j + i);

			// 上半分の三角形
			index.at(k * 3) = count;
			index.at(k * 3 + 1) = count + slices + 2;
			index.at(k * 3 + 2) = count + 1;
			++k;

			// 下半分の三角形
			index.at(k * 3) = count;
			index.at(k * 3 + 1) = count + slices + 1;
			index.at(k * 3 + 2) = count + slices + 2;
			++k;
		}
	}
}


void Shape::createSkyboxCube() {
	this->vertices = 8;
	this->indices = 12;

	//動的配列の確保
	this->position.resize(this->vertices * 3);
	this->index.resize(this->indices * 3);

	float length = 10000.0f;

	// 頂点の位置データ   
	float pv[8][3] =
	{
		{ length, length, length },  // (0)     
	{ length, -length,  length },  // (1)     
	{ length,  -length,  -length },  // (2)     
	{ length,  length, -length },  // (3)     
	{ -length,  length, length },  // (4)     
	{ -length, -length, length },  // (5)     
	{ -length, -length,  -length },  // (6)     
	{ -length,  length,  -length }   // (7)   
	};

	unsigned int iv[12][3] =
	{
		{ 0,1,2 },  // (0)     
	{ 0,2,3 },  // (1)     
	{ 3,2,6 },  // (2)     
	{ 3,6,7 },  // (3)     
	{ 7,6,5 },  // (4)     
	{ 7,5,4 },  // (5)     
	{ 4,5,1 },  // (6)     
	{ 4,1,0 },  // (7)
	{ 0,3,7 },
	{ 0,7,4 },
	{ 1,5,6 },
	{ 1,6,2 }
	};

	for (int i = 0; i < 8; i++) {

		this->position.at(i * 3) = pv[i][0];
		this->position.at(i * 3 + 1) = pv[i][1];
		this->position.at(i * 3 + 2) = pv[i][2];
	}

	for (int i = 0; i < 12; i++)
	{
		//インデックスを求める
		this->index.at(i * 3) = iv[i][0];
		this->index.at(i * 3 + 1) = iv[i][1];
		this->index.at(i * 3 + 2) = iv[i][2];
	}
}

void Shape::createSphereHair(float hairs, float knots, float length, float radius, std::vector<float> &first)
{
	int hs = int(hairs);
	int ks = int(knots);

	this->vertices = hs * ks;

	//動的配列の確保
	this->position.resize(this->vertices * 4);
	this->normal.resize(this->vertices * 4);
	this->texcoord.resize(this->vertices * 4);
	for (int i = 0; i<hs; i++)
	{
		//節点間の長さ
		const float knotlength(length / float(ks - 1));
		//方位角
		const float t(6.283185f * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));
		// 仰角
		const float p(1.5707963f * (49.0f / 50.0f) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));

		float line_vec[3];
		line_vec[0] = cos(p) * cos(t);
		line_vec[1] = sin(p);
		line_vec[2] = cos(p) * sin(t);

		Normalize(line_vec);

		float vp[3];

		//根本の頂点の座標番号を更新
		first.at(i) = knots * float(i);
		for (int j = 0; j < ks; j++)
		{
			int num = 4 * (i * ks + j);
			if (j == 0 || j == 1) {
				position.at(num) = (radius - 0.01f) * line_vec[0];
				position.at(num + 1) = (radius - 0.01f) * line_vec[1];
				position.at(num + 2) = (radius - 0.01f) * line_vec[2];
				position.at(num + 3) = float(j);
			}
			else {
				position.at(num) = knotlength * line_vec[0] + vp[0];
				position.at(num + 1) = knotlength * line_vec[1] + vp[1];
				position.at(num + 2) = knotlength * line_vec[2] + vp[2];
				position.at(num + 3) = float(j);
			}
			normal.at(num) = 0.0f;
			normal.at(num + 1) = 0.0f;
			normal.at(num + 2) = 0.0f;
			normal.at(num + 3) = float(0);

			texcoord.at(num) = 0.0f;
			texcoord.at(num + 1) = 0.0f;
			texcoord.at(num + 2) = 0.0f;
			texcoord.at(num + 3) = float(0);

			vp[0] = position.at(num);
			vp[1] = position.at(num + 1);
			vp[2] = position.at(num + 2);

			float d = distance(vp[0], vp[1], vp[2]);
		}
	}
}

void Shape::createPlaneHair(float hairs, float knots, float length, float radius, std::vector<float> &first)
{
	int hs = int(hairs);
	int ks = int(knots);

	this->vertices = hs * ks;

	//動的配列の確保
	this->position.resize(this->vertices * 4);
	this->normal.resize(this->vertices * 4);
	this->texcoord.resize(this->vertices * 4);
	for (int i = 0; i<hs; i++)
	{
		//節点間の長さ
		const float knotlength(length / float(ks - 1));
		//横
		const float w((float(rand()) / float(RAND_MAX) - 0.5f));
		// 縦
		const float h((float(rand()) / float(RAND_MAX) - 0.5f));

		float line_vec[3];
		line_vec[0] = w;
		line_vec[1] = h;
		line_vec[2] = 0.0f;

		float vp[3];

		//根本の頂点の座標番号を更新
		first.at(i) = knots * float(i);
		for (int j = 0; j < ks; j++)
		{
			int num = 4 * (i * ks + j);
			if (j == 0 || j == 1) {
				position.at(num) = line_vec[0];
				position.at(num + 1) = line_vec[1];
				position.at(num + 2) = line_vec[2];
				position.at(num + 3) = float(j);
			}
			else {
				position.at(num) = knotlength * 0.0f + vp[0];
				position.at(num + 1) = knotlength * 0.0f + vp[1];
				position.at(num + 2) = knotlength * 1.0f + vp[2];
				position.at(num + 3) = float(j);
			}
			normal.at(num) = 0.0f;
			normal.at(num + 1) = 0.0f;
			normal.at(num + 2) = 0.0f;
			normal.at(num + 3) = float(0);

			texcoord.at(num) = 0.0f;
			texcoord.at(num + 1) = 0.0f;
			texcoord.at(num + 2) = 0.0f;
			texcoord.at(num + 3) = float(0);

			vp[0] = position.at(num);
			vp[1] = position.at(num + 1);
			vp[2] = position.at(num + 2);
		}
	}
}

void setCubedata(std::vector<GLubyte> &vd, int size)
{
	int i, j, k;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				int index = i * size * size + j * size + k;
				float a = 1.0f;
				double d = a * 255.0;
				vd.at(index) = static_cast<GLubyte>(d);
			}
		}
	}

}


void setRandomdata(std::vector<GLubyte> &vd, int size)
{
	int i, j, k;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				int index = i * size * size + j * size + k;
				float a = (float(rand()) / float(RAND_MAX));
				double d = a * 255.0;
				vd.at(index) = static_cast<GLubyte>(d);
			}
		}
	}

}

void setVoxeldata(BASIC3D_TYPE t, std::vector<GLubyte> &vd, int size)
{
	switch (t) {
	case VCube:
		setCubedata(vd, size);
		break;
	case VRandom:
		setRandomdata(vd, size);
		break;
	}
}