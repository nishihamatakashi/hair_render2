#include"object3d.h"
#include"shape.h"

void Object3D::onCull_Front() { cull_front = true; cull_back = false; }
void Object3D::onCull_Back() { cull_back = true; cull_front = false; }
void Object3D::onDepth_Test() { depth_test = true; }
void Object3D::offDepth_Test() { depth_test = false; }
void Object3D::onAlpha_Test() { alpha_test = true; }
void Object3D::offAlpha_Test() { alpha_test = false; }
void Object3D::onBillboad() { billboad = true; }
void Object3D::offBillboad() { billboad = false; }
void Object3D::setMaterial(Material &m) { material = &m; }
void Object3D::setType(BASIC3D_TYPE t) { type = t; }
void Object3D::setSType(SHADER_TYPE st) { stype = st; }
void Object3D::setForce(float x, float y, float z) { force[0] = x, force[1] = y, force[2] = z; }

//get function
bool Object3D::getCull_Front() { return cull_front; }
bool Object3D::getCull_Back() { return cull_back; }
bool Object3D::getDepth_Test() { return depth_test; }
bool Object3D::getAlpha_Test() { return alpha_test; }
Matrix* Object3D::getMatrix() { return &m; }
Material* Object3D::getMaterial() { return material; }
BASIC3D_TYPE Object3D::getType() { return type; }
const float* Object3D::getForce() { return force; }

void Object3D::setEnableFunc() 
{
	if (this->cull_front) { glCullFace(GL_FRONT); }
	if (this->cull_back) { glCullFace(GL_BACK); }
	if (this->depth_test) { glEnable(GL_DEPTH_TEST); }
	else { glDisable(GL_DEPTH_TEST); }

	if (this->alpha_test) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void Object3D::setMatrix(Camera &camera)
{
	const float *pos = getPosition();
	const float *rot = getRotation();
	const float *sca = getScale();

	if (billboad)
	{
		loadLookat(camera.getRegard(), camera.getPosition(), camera.getUpvector(), m.model);
		m.model[12] = m.model[13] = m.model[14] = 0.0f;
		Inverse(m.model, m.model);
		Scale(sca[0], sca[1], sca[2], m.model);
		Translate(pos[0], pos[1], pos[2], m.model);

		//texture行列の設定
		loadLookat(camera.getRegard(), camera.getPosition(), camera.getUpvector(), m.texture);
		Inverse(m.texture, m.texture);
		EulerRotate(rot, this->m.texture);

		float mat[16];
		loadEulerRotate(rot, mat);
		Inverse(mat,mat);
		adjoint(mat, m.adjoint);
	}
	else
	{
		loadEulerRotate(rot, m.model);
		Scale(sca[0], sca[1], sca[2], m.model);
		Translate(pos[0], pos[1], pos[2], m.model);

		adjoint(m.model, m.adjoint);

	}

	loadLookat(camera.getPosition(), camera.getRegard(), camera.getUpvector(), this->m.view);

	if (camera.getType() == Ortho)
	{
		const float *par = camera.getOrtho_size();
		loadOrthogonal(par[1], par[0], par[3], par[2], par[5], par[4], this->m.proj);
	}
	else if (camera.getType() == Perth)
	{
		const float *par = camera.getPerth_size();
		loadPerspective(camera.getFovy(), camera.getAspect(), par[1], par[0], m.proj);
	}

	//clip行列の設定
	loadIdentity(this->m.clip);
}

GLuint Basic3D::getVao() { return vao; }
GLuint* Basic3D::getTbo() { return tbo; }
int Basic3D::getVertices() { return this->vertices; }
int Basic3D::getIndices() { return this->indices; }
const int* Basic3D::getResolution() { return this->resolution; }
float* Basic3D::getParameter() { return this->parameter; }
std::vector<float>& Basic3D::getVec_parameter() { return this->vec_parameter; }

void Basic3D::shutdown() {
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(3, vbo);
	glDeleteTextures(3, tbo);
	glDeleteTextures(1, &space_tex);
}

void Basic3D::loadObject3D(BASIC3D_TYPE t, SHADER_TYPE st)
{
	setType(t);
	setSType(st);
	Shape shape(t);
	if (t == Billboad) { onBillboad(); }
	this->vertices = shape.vertices;
	this->indices = shape.indices;

	parameter[4] = float(2 * vertices / 4);
	parameter[5] = 1.5f;

	calcResolution(shape.vertices, resolution);

	// 頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 頂点バッファオブジェクト
	glGenBuffers(3, vbo);

	//頂点座標
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[3]), &shape.position[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//法線
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[3]), &shape.normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//uv座標
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[2]), &shape.texcoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//インデックスの頂点バッファオブジェクト
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint[3]) * shape.indices, &shape.index[0], GL_STATIC_DRAW);

	//TBOを3個作りVBOをバインドしていく
	glGenTextures(3, tbo);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[0]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo[0]);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[1]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo[1]);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[2]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo[2]);

	_glError(__FILE__, __LINE__);
}

void Basic3D::loadObject3D(BASIC3D_TYPE t, SHADER_TYPE st, float hairs, float knots, float length, float radius)
{
	setType(t);
	setSType(st);
	vec_parameter.resize(int(hairs));
	Shape shape(t, hairs, knots, length, radius, vec_parameter);
	parameter[0] = hairs;
	parameter[1] = knots;
	parameter[2] = length;
	parameter[3] = radius;
	parameter[4] = 100;
	parameter[5] = (length + radius);


	parameter[6] = 0.1f;	//硬さ1
	parameter[7] = 2.0f;	//硬さ2
	parameter[8] = 0.0f;	//付着率
	parameter[9] = 0.004f;	//太さ
	parameter[10] = 1.0f;	//方位角方向の粗さ
	parameter[11] = 0.3f;	//仰角方向の粗さ
	parameter[12] = 0.02f;	//オフセット角度
	parameter[13] = 0.5f;	//吸収係数
	parameter[14] = 5.0f;	//円柱の角数
	parameter[15] = 1.0f;	//先のとがり具合
	parameter[16] = 1.0f;	//濡れかどうかのフラグ
	parameter[17] = 0.2f;	//濡れた際の滑らかさ

	v.resize(unsigned int(parameter[4] * parameter[4] * parameter[4] * 4));

	calcResolution(shape.vertices, resolution);

	// 頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 頂点バッファオブジェクト
	glGenBuffers(3, vbo);

	//頂点座標
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[4]), &shape.position[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//速度情報
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[4]), &shape.normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//力情報
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[4]), &shape.texcoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//TBOを3個作りVBOをバインドしていく
	glGenTextures(3, tbo);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[0]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo[0]);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[1]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo[1]);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[2]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo[2]);

	glGenTextures(1, &space_tex);
	glBindTexture(GL_TEXTURE_3D, space_tex);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, GLsizei(parameter[4]), GLsizei(parameter[4]), GLsizei(parameter[4]), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// テクスチャの境界色を設定する (ボリュームの外には何もない)
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, black);

	_glError(__FILE__, __LINE__);
}

const int* Volume3D::getCell_size() { return cell_size; }
GLuint Volume3D::getVao() { return vao; }
GLuint Volume3D::getVdata() { return vdata; }
int Volume3D::getVertices() { return this->vertices; }
float* Volume3D::getParameter() { return this->parameter; }

void Volume3D::shutdown()
{
	glDeleteBuffers(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteTextures(1, &this->vdata);
}

void Volume3D::loadObject3D(BASIC3D_TYPE t, SHADER_TYPE st, int slices, float threshold, int size)
{
	setType(Volume);
	setSType(st);
	Shape shape(Billboad);
	parameter[0] = threshold;
	cell_size[0] = cell_size[1] = cell_size[2] = size;
	vertices = shape.vertices * 3;
	indices = shape.indices;
	onBillboad();

	std::vector<GLubyte> vd;
	vd.resize(size*size*size);

	setVoxeldata(t, vd, size);

	// 頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 頂点バッファオブジェクト
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[3]), &shape.position[0], GL_STATIC_DRAW);
	//結合されている頂点バッファオブジェクトをin変数から参照できるようにする
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//インデックスの頂点バッファオブジェクト
	//頂点バッファオブジェクト
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint[3]) * shape.indices, &shape.index[0], GL_STATIC_DRAW);

	//ボリュームテクスチャの作成
	glGenTextures(1, &vdata);
	glBindTexture(GL_TEXTURE_3D, vdata);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, size, size, size, 0, GL_RED, GL_UNSIGNED_BYTE, &vd[0]);

	// テクスチャの拡大・縮小に線形補間を用いる
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// テクスチャからはみ出た部分には境界色を用いる
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	_glError(__FILE__, __LINE__);
}

void Volume3D::loadObject3D(GLuint tex, SHADER_TYPE st, int slices, float threshold, int size)
{
	setType(Volume);
	setSType(st);
	Shape shape(Billboad);
	parameter[0] = threshold;
	parameter[1] = float(slices);
	cell_size[0] = cell_size[1] = cell_size[2] = size;
	vdata = tex;
	vertices = shape.vertices * 3;
	indices = shape.indices;
	onBillboad();

	// 頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 頂点バッファオブジェクト
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[3]), &shape.position[0], GL_STATIC_DRAW);
	//結合されている頂点バッファオブジェクトをin変数から参照できるようにする
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//インデックスの頂点バッファオブジェクト
	//頂点バッファオブジェクト
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint[3]) * shape.indices, &shape.index[0], GL_STATIC_DRAW);

	_glError(__FILE__, __LINE__);
}

//SkyBox生成
void SkyBox::loadSkybox()
{
	Shape shape(Skybox);

	vertices = shape.vertices;
	indices = shape.indices;

	// 頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 頂点バッファオブジェクト
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices * sizeof(GLfloat[3]), &shape.position[0], GL_STATIC_DRAW);
	//結合されている頂点バッファオブジェクトをin変数から参照できるようにする
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//インデックスの頂点バッファオブジェクト
	//頂点バッファオブジェクト
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint[3]) * shape.indices, &shape.index[0], GL_STATIC_DRAW);
};

void SkyBox::setEnableFunc()
{
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
}

void SkyBox::setMatrix(Camera &camera)
{
	loadSkyLookat(camera.getPosition(), camera.getRegard(), camera.getUpvector(), this->view);
	Inverse(this->view,this->adjo);
	adjoint(this->adjo, this->adjo);
	const float *par = camera.getPerth_size();
	loadPerspective(fovy, camera.getAspect(), par[1], par[0], this->proj);
}

void Fluid3D::loadObject3D(int size)
{
	parameter[0] = 1.0f;
	parameter[1] = 1.0f;
	cell_size = size;

	//追加用データのメモリ確保
	this->add_vel.resize(4 * cell_size * cell_size * cell_size);
	this->add_dens.resize(4 * cell_size * cell_size * cell_size);


	//計算用のテクスチャを4個の作成
	glGenTextures(6, simulate_tex);

	//現在の速度情報
	glBindTexture(GL_TEXTURE_3D, simulate_tex[0]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, cell_size, cell_size, cell_size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	//1ステップ前の速度情報
	glBindTexture(GL_TEXTURE_3D, simulate_tex[1]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, cell_size, cell_size, cell_size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	//現在の密度情報
	glBindTexture(GL_TEXTURE_3D, simulate_tex[2]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, cell_size, cell_size, cell_size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	//1ステップ前の密度情報
	glBindTexture(GL_TEXTURE_3D, simulate_tex[3]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, cell_size, cell_size, cell_size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	//ダイバージェンス情報
	glBindTexture(GL_TEXTURE_3D, simulate_tex[4]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, cell_size, cell_size, cell_size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	//圧力情報
	glBindTexture(GL_TEXTURE_3D, simulate_tex[5]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, cell_size, cell_size, cell_size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	_glError(__FILE__, __LINE__);
}