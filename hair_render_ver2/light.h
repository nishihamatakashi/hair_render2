#pragma once
#include"gameobject.h"

//light�N���X
class Light : public GameObject 
{
private:
	float vector[3] = { -1.0f,-1.0f, -1.0f, };
	float color[3] = { 1.0f,1.0f,1.0f };
	float size[2] = { 1.0f,1.0f };	//0�`100000.0f
	float energy = 1.0f;			//0�`10.0f
	float distance = 25.0f;			//0�`100000.0f
	float spot_size = 60.0f;		//0�`180.0f
	float blend = 1.0f;				//0�`1.0f
	float gamma = 1.0f;				//0�`1.0f
	LIGHT_TYPE type = Point;		//0�FPoint,1�FSpot,2�FArea,3�FHemi,4�FSun
public:
	//constructor
	Light() {}

	//destructor
	~Light() {}

	//���z�֐�
	void init()override {}
	void shutdown()override {}
	void update()override {}

	//set function
	void setType(LIGHT_TYPE t);
	void setEnergy(float value);
	void setDistance(float value);
	void setVector(float x, float y, float z);
	void setSpot_size(float value);
	void setBlend(float value);
	void setSize(float x, float y);
	void setGamma(float value);
	void setColor(float r, float g, float b);

	//get function
	const LIGHT_TYPE getType();
	const float getEnergy();
	const float getDistance();
	const float* getVector();
	const float getSpot_size();
	const float getBlend();
	const float* getSize();
	const float getGamma();
	const float* getColor();
};