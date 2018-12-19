#pragma once
#include"hr_math.h"

//ゲームオブジェクトのスーパークラス
class GameObject
{
private:
	std::string name;									//名前
	bool select = false;						//選択状態
	bool active = true;							//active状態
	float position[3] = { 0.0f,0.0f,0.0f };		//位置
	float rotation[3] = { 0.0f,0.0f,0.0f };		//姿勢
	float scale[3] = { 1.0f,1.0f,1.0f };		//拡縮
	float preposition[3] = { 0.0f,0.0f,0.0f };	//1ステップ前の位置
public:
	//コンストラクタ(何もしない)
	GameObject() {}

	//デストラクタ(何もしない)
	~GameObject() {}

	//仮想関数
	virtual void init() {}
	virtual void shutdown() {}
	virtual void update() {}

	//デストロイ関数
	void destroy() { active = false; }

	//get関数
	std::string& getName();
	const float* getPosition();
	const float* getRotation();
	const float* getScale();
	const bool getActive();
	const bool getSelect();

	//set関数
	void onSelect();
	void offSelect();
	void setName(const char* n);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void addPosition(float x, float y, float z);
	void addRotation(float x, float y, float z);
	void addScale(float x, float y, float z);
	void setPrePosition();
};