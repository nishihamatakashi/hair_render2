#include "window.h"

//�E�C���h�E�̏���������
void Window::init(int width, int height, const char* title)
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	//�r���[�|�[�g�̐ݒ�
	glViewport(100, 50, 300, 300);

	if (window == NULL) {
		//�E�C���h�E���쐬�ł��Ȃ�����
		std::cerr << "Can't create GLFW window" << std::endl;
		exit(1);
	}

	//���݂̃E�C���h�E�������Ώۂɂ���
	glfwMakeContextCurrent(window);

	//GLEW������������
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		//GLEW�̏������Ɏ��s����
		std::cerr << "Can't initialize GLEW" << std::endl;
		exit(1);
	}

	//���������̃^�C�~���O��҂�
	glfwSwapInterval(0);

	//���̃C���X�^���X��this�|�C���^���L�^���Ă���
	glfwSetWindowUserPointer(window, this);

	//�E�C���h�E�̃T�C�Y�ύX���ɌĂяo�������̓o�^
	glfwSetWindowSizeCallback(window, resize);

	//�}�E�X�z�C�[�����쎞�ɌĂяo�������̓o�^
	glfwSetScrollCallback(window, wheel);

	//�L�[�{�[�h���쎞�ɌĂяo�������̓o�^
	glfwSetKeyCallback(window, keyboard);

	//�J�����E�C���h�E�̏����ݒ�
	resize(window, width, height);
}

//�J���[�o�b�t�@�����ւ��ăC�x���g�����o��
void Window::swapBuffers()
{
	mouse_wheel = 0.0f;
	//�J���[�o�b�t�@�̓���ւ�
	glfwSwapBuffers(window);

	// �C�x���g�����o��
	glfwPollEvents();

	//�}�E�X�J�[�\���̈ʒu���擾����
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	//�}�E�X�J�[�\���̐��K���f�o�C�X���W�n��ł̈ʒu�����߂�
	mouse_pos[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
	mouse_pos[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];

	//�}�E�X�̏�Ԃ�����D
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

//�E�C���h�E�T�C�Y�ύX���̃��T�C�Y����
void Window::resize(GLFWwindow *const window, int width, int height)
{
	//�E�C���h�E�S�̂��r���[�|�[�g�ɐݒ肷��
	glViewport(0, 0, width, height);

	//���̃C���X�^���X��this�|�C���^�𓾂�
	Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

	if (instance != NULL)
	{
		//�C���X�^���X���ێ�����c������s�i����
		instance->size[0] = static_cast<GLfloat>(width);
		instance->size[1] = static_cast<GLfloat>(height);
		instance->updateScale();
	}
}

void Window::wheel(GLFWwindow *window, double x, double y)
{
	//���̃C���X�^���X��this�|�C���^�𓾂�
	Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
	instance->mouse_wheel = static_cast<GLfloat>(y);
}

//�L�[�{�[�h���쎞�̏���
void Window::keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	//���̃C���X�^���X��this�|�C���^�𓾂�
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