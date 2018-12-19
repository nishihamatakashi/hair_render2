#include "window.h"

//ウインドウの初期化処理
void Window::init(int width, int height, const char* title)
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	//ビューポートの設定
	glViewport(100, 50, 300, 300);

	if (window == NULL) {
		//ウインドウが作成できなかった
		std::cerr << "Can't create GLFW window" << std::endl;
		exit(1);
	}

	//現在のウインドウを処理対象にする
	glfwMakeContextCurrent(window);

	//GLEWを初期化する
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		//GLEWの初期化に失敗した
		std::cerr << "Can't initialize GLEW" << std::endl;
		exit(1);
	}

	//垂直同期のタイミングを待つ
	glfwSwapInterval(0);

	//このインスタンスのthisポインタを記録しておく
	glfwSetWindowUserPointer(window, this);

	//ウインドウのサイズ変更時に呼び出す処理の登録
	glfwSetWindowSizeCallback(window, resize);

	//マウスホイール操作時に呼び出す処理の登録
	glfwSetScrollCallback(window, wheel);

	//キーボード操作時に呼び出す処理の登録
	glfwSetKeyCallback(window, keyboard);

	//開いたウインドウの初期設定
	resize(window, width, height);
}

//カラーバッファを入れ替えてイベントを取り出す
void Window::swapBuffers()
{
	mouse_wheel = 0.0f;
	//カラーバッファの入れ替え
	glfwSwapBuffers(window);

	// イベントを取り出す
	glfwPollEvents();

	//マウスカーソルの位置を取得する
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	//マウスカーソルの正規化デバイス座標系上での位置を求める
	mouse_pos[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
	mouse_pos[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];

	//マウスの状態を見る．
	bool lb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE;
	bool mb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) != GLFW_RELEASE;
	bool rb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) != GLFW_RELEASE;

	mouse_button[0] = lb == true ? true : false;
	mouse_button[1] = mb == true ? true : false;
	mouse_button[2] = rb == true ? true : false;

	updateMouse_move();
	updateMouse_prepos();

	if (keystatus[0] == GLFW_PRESS && pre_keystatus[0] == GLFW_PRESS)
	{
		keystatus[0] = GLFW_REPEAT;
	}
	else if (keystatus[0] == GLFW_PRESS && pre_keystatus[0] == GLFW_REPEAT)
	{
		keystatus[0] = GLFW_REPEAT;
	}

	pre_keystatus[0] = keystatus[0];
	pre_keystatus[1] = keystatus[1];

	framecount++;
}

//ウインドウサイズ変更時のリサイズ処理
void Window::resize(GLFWwindow *const window, int width, int height)
{
	//ウインドウ全体をビューポートに設定する
	glViewport(0, 0, width, height);

	//このインスタンスのthisポインタを得る
	Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

	if (instance != NULL)
	{
		//インスタンスが保持する縦横比を行進する
		instance->size[0] = static_cast<GLfloat>(width);
		instance->size[1] = static_cast<GLfloat>(height);
		instance->updateScale();
	}
}

void Window::wheel(GLFWwindow *window, double x, double y)
{
	//このインスタンスのthisポインタを得る
	Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
	instance->mouse_wheel = static_cast<GLfloat>(y);
}

//キーボード操作時の処理
void Window::keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	//このインスタンスのthisポインタを得る
	Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
	if (instance != NULL)
	{
		if (instance->keystatus[0] == GLFW_PRESS && action == GLFW_PRESS)
		{
			instance->keystatus[0] = GLFW_REPEAT;
		}
		else if (instance->keystatus[0] == GLFW_REPEAT && action == GLFW_PRESS)
		{
			instance->keystatus[0] = GLFW_REPEAT;
		}
		else {
			instance->keystatus[0] = action;
			instance->keystatus[1] = scancode;
		}
	}
}