#pragma once
#include"draw_shader.h"
#include"compute_shader.h"
#include"texture.h"

//Manager系のスーパークラス
class Manager
{
private:
protected:

	//連想配列を用いたインスタンスの取得関数
	template<typename T, typename U>U& getObject(const char *name, T &data) {
		bool check = checkName<T>(name, data);
		assert(check && "you will get the non-exit");
		return *data[name].get();
	}

	//連想配列を用いたインスタンスの消去関数
	template<typename T>void deleteObject(const char* name, T &data)
	{
		bool check = checkName<T>(name, data);
		assert(check && "you will get the non-exit");
		data[name].get()->shutdown();
		data.erase(name);
	}

	//システムをシャットダウンする際の処理
	template<typename T>void shutdown(T &data)
	{
		for (auto &x : data) {
			x.second->shutdown();
			size--;
		}
		data.clear();
	}

	//新しいインスタンスを加える際の処理
	template<typename T>void addObject(const char *name, T &data)
	{
		bool check = checkName<T>(name, data);
		assert(!check && "you will create the same name objecct：");
	}

	//manager毎のリソース表示の仮装関数
	virtual void showResource() {};

	//manager毎のリソースの数値設定の仮装関数
	virtual void setting() {};
public:
	size_t size;
};

//ライトManager
class LightManager : public Manager
{
private:
public:
	//連想配列で管理
	std::map<const char*, std::unique_ptr<Light>> lights;

	//新しいライトのインスタンスを追加する関数
	Light & addLight(const char *name, LIGHT_TYPE type)
	{
		addObject<std::map<const char*, std::unique_ptr<Light>>>(name, lights);
		Light* e = new Light();
		e->setType(type);
		e->setName(name);
		std::unique_ptr<Light>uPtr{ e };
		lights.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		size++;
		return *e;
	}

	//指定した名前のインスタンスを取得
	Light& getLight(const char *name) {
		return getObject<std::map<const char*, std::unique_ptr<Light>>, Light>(name, lights);
	}

	//指定した名前のインスタンスを消去
	void deleteLight(const char *name) {
		deleteObject<std::map<const char*, std::unique_ptr<Light>>>(name, lights);
	}

	//システムをシャットダウンする際の処理
	void shutdownLight()
	{
		shutdown<std::map<const char*, std::unique_ptr<Light>>>(lights);
	}

	//毎フレーム何らかの更新を行う場合はここに記述
	void update() {}

	//リソースを表示する
	void showResource()override;
};

//
class Object3DManager : public Manager
{
private:
public:
	std::map<const char*, std::unique_ptr<Object3D>> object3ds;

	template<typename T>T &addObject3D(const char* name, BASIC3D_TYPE type,SHADER_TYPE stype)
	{
		T* e = new T();
		e->loadObject3D(type,stype);
		e->setName(name);
		std::unique_ptr<Object3D>uPtr{ e };
		object3ds.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		size++;
		return *e;
	}

	template<typename T>T &addObject3D(const char *name, BASIC3D_TYPE type, SHADER_TYPE stype,float hairs, float knots, float length, float radius)
	{
		T* e = new T();
		e->loadObject3D(type, stype, hairs, knots, length, radius);
		e->setName(name);
		std::unique_ptr<Object3D>uPtr{ e };
		object3ds.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		size++;
		return *e;
	}

	template<typename T>T &addObject3D(const char *name, BASIC3D_TYPE t, SHADER_TYPE stype, int slices, float threshold, int size)
	{
		T* e = new T();
		e->loadObject3D(t, stype, slices, threshold, size);
		e->setName(name);
		std::unique_ptr<Object3D>uPtr{ e };
		object3ds.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		this->size++;
		return *e;
	}

	template<typename T>T &addObject3D(const char *name, GLuint tex, SHADER_TYPE stype, int slices, float threshold, int size)
	{
		T* e = new T();
		e->loadObject3D(tex, stype, slices, threshold, size);
		e->setName(name);
		std::unique_ptr<Object3D>uPtr{ e };
		object3ds.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		this->size++;
		return *e;
	}

	template<typename T>T& getObject3D(const char *name) {
		bool check = checkName<std::map<const char*, std::unique_ptr<Object3D>>>(name, object3ds);
		assert(check && "you will get the non-exit");
		auto ptr = shaders[name].get();
		return *static_cast<T*>(ptr);
	}

	void deleteObject3D(const char *name) {
		deleteObject<std::map<const char*, std::unique_ptr<Object3D>>>(name, object3ds);
	}

	void shutdownBasic3D()
	{
		shutdown<std::map<const char*, std::unique_ptr<Object3D>>>(object3ds);
	}
	void update() {}
	void showResource()override;
};

class TextureManager : public Manager
{
private:
public:
	std::map<const char*, GLuint> textures;

	GLuint getTexture(const char *name) {
		bool check = checkName<std::map<const char*, GLuint>>(name, textures);
		assert(check && "you will get the non-exit");
		return textures[name];
	}

	void deleteTexture(const char *name) {
		bool check = checkName<std::map<const char*, GLuint>>(name, textures);
		assert(check && "you will get the non-exit");
		glDeleteTextures(1, &textures[name]);
		textures.erase(name);
		size--;
	}

	void shutdownTexture()
	{
		for (auto &x : textures) {
			glDeleteTextures(1, &x.second);
			size--;
		}
		textures.clear();
	}

	GLuint addTexture(const char *tsrc, TEXTURE_TYPE type)
	{
		bool check = checkName<std::map<const char*, GLuint>>(tsrc, textures);
		assert(!check && "you will create the same name objecct：");
		GLuint *tex = &textures[tsrc];
		createTexture(*tex, tsrc, type);
		size++;
		return *tex;
	}

	GLuint addTexture(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, TEXTURE_TYPE type)
	{
		bool check = checkName<std::map<const char*, GLuint>>(px, textures);
		assert(!check && "you will create the same name objecct：");
		GLuint *tex = &textures[px];
		createTexture(*tex, px, nx, py, ny, pz, nz, type);
		size++;
		return *tex;
	}

	void createTexture(GLuint &tex, const char* tsrc, TEXTURE_TYPE t)
	{
		switch (t) {
		case ColorMap:
			LoadImage(tex, tsrc);
			break;
		case HeightMap:
			LoadHeight(tex, tsrc, 0.5f);
			break;
		case TestMap:
			LoadTestMap(tex, tsrc);
		default:
			break;
		}
		_glError(__FILE__, __LINE__);
	}
	void createTexture(GLuint &tex, const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, TEXTURE_TYPE t)
	{
		switch (t) {
		case CubeMap:
			LoadCubeImage(tex, px, nx, py, ny, pz, nz);
			break;
		default:
			return;
			break;
		}
		_glError(__FILE__, __LINE__);
	}

	void update() {}

	void showResource()override;
};

class MaterialManager : public Manager
{
private:
public:
	std::map<const char*, std::unique_ptr<Material>> materials;
	Material & addMaterial(const char *name)
	{

		Material* e = new Material();
		std::unique_ptr<Material>uPtr{ e };
		materials.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		size++;
		return *e;
	}

	Material& getMaterial(const char *name) {
		return getObject<std::map<const char*, std::unique_ptr<Material>>, Material>(name, materials);
	}

	void deleteMaterial(const char *name) {
		deleteObject<std::map<const char*, std::unique_ptr<Material>>>(name, materials);
	}

	void shutdownMaterial()
	{
		shutdown<std::map<const char*, std::unique_ptr<Material>>>(materials);
	}
	void update(){}
	void showResource()override;
};

class ShaderManager : Manager
{
private:
public:
	std::map<const char*, std::unique_ptr<Shader>> shaders;
	template<typename T>T& addShader(const char *name, const char* vert, const char* frag, const char* geom = 0, const char* tessc = 0, const char* tesse = 0)
	{
		T* e = new T();
		e->createShader(vert, frag, geom, tessc, tesse);
		std::unique_ptr<Shader>uPtr{ e };
		shaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(uPtr))
		);
		size++;
		return *e;
	}

	template<typename T>T& getShader(const char *name) {
		bool check = checkName<std::map<const char*, std::unique_ptr<Shader>>>(name, shaders);
		assert(check && "you will get the non-exit");
		auto ptr = shaders[name].get();
		return *static_cast<T*>(ptr);
	}

	void deleteShader(const char *name) {
		deleteObject<std::map<const char*, std::unique_ptr<Shader>>>(name, shaders);
	}

	void shutdownShader()
	{
		shutdown<std::map<const char*, std::unique_ptr<Shader>>>(shaders);
	}
	void update() {}
	void showResource()override;
};