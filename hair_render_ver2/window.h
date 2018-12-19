#pragma once
/*
**ウインドウ関連の処理
*/

#include"object3d.h"

class Window
{
private:
	//ウインドウのハンドル
	GLFWwindow * window;

	//ウインドウのサイズ，スケール
	GLfloat size[2] = { 400.0f,300.0f };
	GLfloat scale[2] = { 50.0f,50.0f };
	GLfloat scale_p = 1.0f;

	//mouseの情報
	GLfloat mouse_move[2] = { 0.0f,0.0f };
	GLfloat mouse_pos[2] = { 0.0f,0.0f };
	GLfloat mouse_prepos[2] = { 0.0f,0.0f };
	bool mouse_button[3] = { false,false,false };
	GLfloat mouse_move_p = 1.0f;
	GLfloat mouse_wheel = 0.0f;
	GLfloat mouse_wheel_p = 1.0f;

	//キーボードの情報
	GLint keystatus[2] = { GLFW_RELEASE,GLFW_KEY_UNKNOWN };
	GLint pre_keystatus[2] = { GLFW_RELEASE,GLFW_KEY_UNKNOWN };

	int framecount = 0;

	//更新関数
	void updateScale() {
		scale[0] = scale_p * 1000.0f / static_cast<GLfloat>(size[0]);
		scale[1] = scale_p * 1000.0f / static_cast<GLfloat>(size[1]);
	}

	//マウスの前回の位置を更新
	void updateMouse_prepos()
	{
		mouse_prepos[0] = mouse_pos[0];
		mouse_prepos[1] = mouse_pos[1];
	}

	//マウスの移動距離を更新
	void updateMouse_move()
	{
		mouse_move[0] = mouse_pos[0] - mouse_prepos[0];
		mouse_move[1] = mouse_pos[1] - mouse_prepos[1];
	}
public:

	//コンストラクタ
	Window() {};

	//デストラクタ
	virtual ~Window() {};

	void init(int width, int height, const char* title);
	void shutdown() { glfwDestroyWindow(window); };

	//ウインドウが閉じているかの判定
	const int shouldClose()
	{
		//ウインドウを消去する
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//escキーを押したら終了，ウインドウを閉じても終了
		return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
	}

	//バッファの入れ替え時の処理
	void swapBuffers();

	//get関数
	GLFWwindow* getWindow() { return window; }
	const GLfloat* getSize() { return size; }
	const GLfloat* getScale() { return scale; }
	const GLfloat* getLocation() { return mouse_pos; }
	const GLfloat* getMouse_move() { return mouse_move; }
	const bool* getMouse_Button() { return mouse_button; }
	const GLint* getKeystatus() { return keystatus; }
	const GLfloat getMouse_wheel() { return mouse_wheel; }

	//コールバック関数
	static void resize(GLFWwindow *const window, int width, int height);					//ウインドウをリサイズ時の関数
	static void wheel(GLFWwindow *window, double x, double y);								//ホイール操作時の関数
	static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);	//キーボードをタイプ時の関数

																							//animate
	void mouse_object_move(Camera &camera, Basic3D &obj)
	{
		if (mouse_button[0])
		{
			float p[3];
			const float mp[2] = { mouse_move[0], mouse_move[1] };
			float dir[3] = { camera.getDirection()[0],camera.getDirection()[1],camera.getDirection()[2] };
			mouseTranslate(mp[0], mp[1], dir, p);
			obj.addPosition(p[0], p[1], p[2]);
		}
	}
};