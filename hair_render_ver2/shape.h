#pragma once
#include"hr_math.h"

class Shape
{
public:
	size_t vertices = 0;
	size_t indices = 0;
	std::vector<float> position;
	std::vector<float> normal;
	std::vector<float> texcoord;
	std::vector<unsigned int> index;

	//コンストラクタ
	Shape(BASIC3D_TYPE t);
	Shape(BASIC3D_TYPE t, float hairs, float knots, float length, float radius, std::vector<float> &first);

	//デストラクタ
	~Shape() {}
private:
	void createPlane();
	void createCube();
	void createSphere(int slices, int stacks);
	void createSkyboxCube();
	void createBillboad();
	void createSphereHair(float hairs, float knots, float length, float radius, std::vector<float> &first);
	void createPlaneHair(float hairs, float knots, float length, float radius, std::vector<float> &first);

};

extern void setVoxeldata(BASIC3D_TYPE t, std::vector<GLubyte> &vd, int size);
