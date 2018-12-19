#pragma once
#include "manager.h"
#include "window.h"

//hair_renderの基本クラス
class HR
{
public:

	//managerのインスタンスを作成
	TextureManager texture_manager;
	MaterialManager material_manager;
	LightManager light_manager;
	Object3DManager object3d_manager;
	ShaderManager shader_manager;

	//singleton objectインスタンスの作成
	Window window;
	Camera camera;
	ImGuiIO* io = nullptr;
	SkyBox skybox;
	//このシステムの設定変数
	const char* glsl_version = "#version 130";
	bool showgui = true;

	//初期化処理
	bool init();

	//毎回更新する関数
	void update();

	//修了処理
	void shutdown();

	//何かしらのオブジェクト動かす関数
	void object_move(Basic3D& obj);

	//windowの閉じたかどうか処理
	bool shouldClose();

	//GUIの表示関数
	void showGUI();
	void showResource();
	void ObjectSetting();
};
