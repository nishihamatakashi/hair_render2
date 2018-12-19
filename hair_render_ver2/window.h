#pragma once
/*
**�E�C���h�E�֘A�̏���
*/

#include"object3d.h"

class Window
{
private:
	//�E�C���h�E�̃n���h��
	GLFWwindow * window;

	//�E�C���h�E�̃T�C�Y�C�X�P�[��
	GLfloat size[2] = { 400.0f,300.0f };
	GLfloat scale[2] = { 50.0f,50.0f };
	GLfloat scale_p = 1.0f;

	//mouse�̏��
	GLfloat mouse_move[2] = { 0.0f,0.0f };
	GLfloat mouse_pos[2] = { 0.0f,0.0f };
	GLfloat mouse_prepos[2] = { 0.0f,0.0f };
	bool mouse_button[3] = { false,false,false };
	GLfloat mouse_move_p = 1.0f;
	GLfloat mouse_wheel = 0.0f;
	GLfloat mouse_wheel_p = 1.0f;

	//�L�[�{�[�h�̏��
	GLint keystatus[2] = { GLFW_RELEASE,GLFW_KEY_UNKNOWN };
	GLint pre_keystatus[2] = { GLFW_RELEASE,GLFW_KEY_UNKNOWN };

	int framecount = 0;

	//�X�V�֐�
	void updateScale() {
		scale[0] = scale_p * 1000.0f / static_cast<GLfloat>(size[0]);
		scale[1] = scale_p * 1000.0f / static_cast<GLfloat>(size[1]);
	}

	//�}�E�X�̑O��̈ʒu���X�V
	void updateMouse_prepos()
	{
		mouse_prepos[0] = mouse_pos[0];
		mouse_prepos[1] = mouse_pos[1];
	}

	//�}�E�X�̈ړ��������X�V
	void updateMouse_move()
	{
		mouse_move[0] = mouse_pos[0] - mouse_prepos[0];
		mouse_move[1] = mouse_pos[1] - mouse_prepos[1];
	}
public:

	//�R���X�g���N�^
	Window() {};

	//�f�X�g���N�^
	virtual ~Window() {};

	void init(int width, int height, const char* title);
	void shutdown() { glfwDestroyWindow(window); };

	//�E�C���h�E�����Ă��邩�̔���
	const int shouldClose()
	{
		//�E�C���h�E����������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//esc�L�[����������I���C�E�C���h�E����Ă��I��
		return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
	}

	//�o�b�t�@�̓���ւ����̏���
	void swapBuffers();

	//get�֐�
	GLFWwindow* getWindow() { return window; }
	const GLfloat* getSize() { return size; }
	const GLfloat* getScale() { return scale; }
	const GLfloat* getLocation() { return mouse_pos; }
	const GLfloat* getMouse_move() { return mouse_move; }
	const bool* getMouse_Button() { return mouse_button; }
	const GLint* getKeystatus() { return keystatus; }
	const GLfloat getMouse_wheel() { return mouse_wheel; }

	//�R�[���o�b�N�֐�
	static void resize(GLFWwindow *const window, int width, int height);					//�E�C���h�E�����T�C�Y���̊֐�
	static void wheel(GLFWwindow *window, double x, double y);								//�z�C�[�����쎞�̊֐�
	static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);	//�L�[�{�[�h���^�C�v���̊֐�

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