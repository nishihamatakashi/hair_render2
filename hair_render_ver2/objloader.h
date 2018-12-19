#pragma once
#include "hr_type.h"

struct Vec3 
{
	float x;
	float y;
	float z;
};

struct Vec2 
{
	float u;
	float v;
};

struct Face 
{
	int v;
	int vt;
	int vn;
};

struct MaterialData 
{
	std::string texture_name;
	float kambient[3];
	float kdiffuse[3];
	float kspecular[3];
	float kshi;
};

struct ObjLabel 
{
	char matlib[8] = "mtllib ";
	char size[6] = "size ";
 	char objgroup[3] = "o ";
	char usemtl[8] = "usemtl ";
	char vertex[3] = "v ";
	char texcoord[4] = "vt ";
	char normal[4] = "vn ";
	char face[3] = "f ";
};

class ObjData 
{
private:
	std::string group_name;
	std::string material_name;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;
	std::vector <std::vector<Face>> faces;
	MaterialData material;
public:
	void setVerticesSize(int size);
	void setNormalsSize(int size);
	void setTexcoordsSize(int size);
	void setFacesSize(int size);
	void setVertices(int n,float x,float y,float z);
	void setTexcoords(int n,float u,float v);
	void setNormals(int n,float x,float v,float z);
	void setFaces(int n, std::vector<Face>& data);
	void setMaterial(MaterialData &data);
	void setGroup_name(std::string& code);
	void setMaterial_name(std::string& code);
};

class ObjDataGroup 
{
private:
	std::string file_name;
	std::string material_file_name;
	std::map<std::string, std::unique_ptr<ObjData>> objdata_group;
	std::string now_group_name;
public:
	int setObjData(const char* name);
	void setFile_name(const char* code);
	void setMaterial_File_name(std::string& code);
	void setStringtoMatlib(std::string& code);
	void setStringtoGroup(std::string& code);
	void setStringtoSize(std::string& code);
	void setStringtoUsemtl(std::string& code);
	void setStringtoVertex(std::string& code,size_t count);
	void setStringtoNormal(std::string& code);
	void setStringtoTexcoord(std::string& code);
	void setStringtoFace(std::string& code);
};
