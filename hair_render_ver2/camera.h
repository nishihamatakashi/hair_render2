#pragma once
#include"gameobject.h"

//カメラクラス
class Camera : public GameObject
{
private:
	//right，left，top，bottom，far，near
	float ortho_size[6] = { 1.0f,-1.0f,1.0f,-1.0f,1.0f,-1.0f };
	float regard[3] = { 0.0f,0.0f,0.0f };
	float upvector[3] = { 0.0f,-1.0f,0.0f };
	float direction[3] = { 0.0f,0.0f,0.0f };
	float perth_size[2] = { 12000,1 };
	float aspect = 1;
	float fovy = 60;
	CAMERA_TYPE type = Perth;
public:
	//constructor
	Camera() {};

	//destructor
	~Camera() {};

	void init()override {}
	void shutdown()override {}
	void update()override;

	//setfunction
	void setType(CAMERA_TYPE t);
	void setRegard(float x, float y, float z);
	void setUpvector(float x, float y, float z);
	void setDirection(float x, float y, float z);
	void setPerth_size(float a, float f, float far, float near);
	void setOrtho_size(float r, float l, float t, float b, float f, float n);

	//get function
	const CAMERA_TYPE getType();
	const float* getRegard();
	const float* getUpvector();
	const float* getDirection();
	const float getAspect();
	const float getFovy();
	const float* getPerth_size();
	const float* getOrtho_size();

	//camera animation
	void ZoomFunc(float w);					//カメラをズームしたいとき
	void RotateFunc(float x, float y);		//回転させたいとき
	void TranslateFunc(float x, float y);	//並行移動するとき
};