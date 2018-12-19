#include "objloader.h"

void ObjData::setVerticesSize(int size) 
{
	vertices.resize(size);
}

void ObjData::setNormalsSize(int size)
{
	normals.resize(size);
}

void ObjData::setTexcoordsSize(int size)
{
	texcoords.resize(size);
}

void ObjData::setFacesSize(int size)
{
	faces.resize(size);
}

void ObjData::setVertices(int n, float x, float y, float z) 
{
	vertices.at(n).x = x;
	vertices.at(n).y = y;
	vertices.at(n).z = z;
}

void ObjData::setNormals(int n, float x, float y, float z)
{
	normals.at(n).x = x;
	normals.at(n).y = y;
	normals.at(n).z = z;
}

void ObjData::setTexcoords(int n, float u,float v)
{
	texcoords.at(n).u = u;
	texcoords.at(n).v = v;
}

void ObjData::setFaces(int n, std::vector<Face>& data) 
{
	faces.at(n).resize(data.size());
	for (size_t i = 0; i < data.size(); i++) {
		faces.at(n).at(i).v = data.at(i).v;
		faces.at(n).at(i).vt = data.at(i).vt;
		faces.at(n).at(i).vn = data.at(i).vn;
	}
}

void ObjData::setMaterial(MaterialData& data) 
{
	material.texture_name = data.texture_name;
	for (int i = 0; i < 3;i++) {
		material.kambient[i] = data.kambient[i];
		material.kdiffuse[i] = data.kdiffuse[i];
		material.kspecular[i] = data.kspecular[i];
	}
	material.kshi = data.kshi;
}

void ObjData::setGroup_name(std::string& code) 
{
	group_name = code;
}

void ObjData::setMaterial_name(std::string& code)
{
	material_name = code;
}

void ObjDataGroup::setFile_name(const char* code)
{
	file_name = code;
}

void ObjDataGroup::setMaterial_File_name(std::string& code)
{
	material_file_name = code;
}

int ObjDataGroup::setObjData(const char* name)
{
	setFile_name(name);
	std::ifstream ifs(name);
	std::string str;
	if (ifs.fail())
	{
		std::cerr << "failed reader file :" << name << std::endl;
		return -1;
	}

	ObjLabel obj_label;

	size_t count[4] = {0,0,0,0};

	while (std::getline(ifs,str))
	{
		if (str.find(obj_label.matlib) != std::string::npos)
		{
			setStringtoMatlib(str);
		}
		else if (str.find(obj_label.objgroup) != std::string::npos)
		{
			setStringtoGroup(str);
		}
		else if (str.find(obj_label.size) != std::string::npos)
		{
			setStringtoSize(str);
		}
		else if (str.find(obj_label.usemtl) != std::string::npos)
		{
			setStringtoUsemtl(str);
		}
		else if (str.find(obj_label.vertex) != std::string::npos)
		{
			
		}
		else if (str.find(obj_label.normal) != std::string::npos)
		{
			
		}
		else if (str.find(obj_label.texcoord) != std::string::npos)
		{
			
		}
		else if (str.find(obj_label.face) != std::string::npos)
		{
			
		}
	}
	return 0;
}

void ObjDataGroup::setStringtoMatlib(std::string& code)
{
	size_t word_count = 0;
	size_t word_index = 0;
	std::string s;
	for (size_t i = 0; i < code.size();i++) {
		if (code.at(i) == ' ') {
			word_index = i + 1;
			break;
		}
	}
	word_count = code.size() - word_index;
	s.resize(word_count);

	for (size_t i = 0; i < word_count; i++) {

		s.at(i) = code.at(i + word_index);
	}
	setMaterial_File_name(s);
}

void ObjDataGroup::setStringtoGroup(std::string& code) 
{
	size_t word_count = 0;
	size_t word_index = 0;
	std::string s;
	for (size_t i = 0; i < code.size(); i++) {
		if (code.at(i) == ' ') {
			word_index = i + 1;
			break;
		}
	}
	word_count = code.size() - word_index;
	s.resize(word_count);

	for (size_t i = 0; i < word_count; i++) {

		s.at(i) = code.at(i + word_index);
	}
	
	ObjData* e = new ObjData();
	std::unique_ptr<ObjData>uPtr{ e };
	objdata_group.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(s),
		std::forward_as_tuple(std::move(uPtr))
	);
	objdata_group[s].get()->setGroup_name(s);
	now_group_name = s;
}

void ObjDataGroup::setStringtoSize(std::string& code) 
{
	
}

void ObjDataGroup::setStringtoUsemtl(std::string& code) 
{
	size_t word_count = 0;
	size_t word_index = 0;
	std::string s;
	for (size_t i = 0; i < code.size(); i++) {
		if (code.at(i) == ' ') {
			word_index = i + 1;
			break;
		}
	}
	word_count = code.size() - word_index;
	s.resize(word_count);

	for (size_t i = 0; i < word_count; i++) {

		s.at(i) = code.at(i + word_index);
	}
	objdata_group[now_group_name].get()->setMaterial_name(s);
}

void ObjDataGroup::setStringtoVertex(std::string& code,size_t count) 
{
	bool flag = false;
	size_t word_count[3] = { 0,0,0 };
	size_t word_index[3] = { 0,0,0 };
	std::string s[3];
	for (size_t i = 0; i < code.size(); i++) {
		if (code.at(i) == ' ') {
			word_index[count] = i + 1;
			count++;
		}
	}
	word_count[0] = word_index[1] - word_index[0] - 1;
	word_count[1] = word_index[2] - word_index[1] - 1;
	word_count[2] = code.size() - word_index[2];
	
	s[0].resize(word_count[0]);
	s[1].resize(word_count[1]);
	s[2].resize(word_count[2]);

	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < word_count[i]; j++) {
			s[i].at(j) = code.at(j + word_index[i]);
		}
	}
	float x = std::stof(s[0]);
	float y = std::stof(s[1]);
	float z = std::stof(s[2]);
	objdata_group[now_group_name].get()->setVertices(count, x, y, z);
}