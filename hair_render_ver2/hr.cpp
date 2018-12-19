#include"hr.h"

bool HR::init()
{

	//カメラパラメータの設定
	const GLfloat *const size(window.getSize());
	const GLfloat *scale(window.getScale());
	const GLfloat fovy = (scale[0] + scale[1]) / 2;
	const GLfloat aspect(size[0] / size[1]);
	camera.setPerth_size(aspect, fovy, 60000.0f, 1.0f);
	camera.setPosition(0.0f, 0.0f, 20.0f);

	//GLFWを初期化
	glewExperimental = GL_TRUE;
	if (glfwInit() == GL_FALSE) {
		//初期化に失敗
		std::cerr << "Can't initialize GLFW" << std::endl;
		return false;
	};
	//プログラム終了時GLFWの処理内容を破棄
	atexit(glfwTerminate);

	//OpenGLの機能を指定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//managerの初期化
	window.init(1500, 1200, "takashi_graphic");


	//背景色を指定
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//imguiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;

	//guiを表示する際の設定
	if (showgui) {
		//init imGui for GLFW and OpenGL
		ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		// Setup style
		ImGui::StyleColorsDark();
	}

	//点の大きさ
	glPointSize(5.0f);

	
	shader_manager.addShader<SkyboxShader>("sbs","skybox.vert","skybox.frag");
	skybox.loadSkybox();

	//opengl上にエラーがないか
	_glError(__FILE__, __LINE__);
	return true;
}

void HR::update()
{
	
	//managerの更新
	texture_manager.update();
	material_manager.update();
	light_manager.update();
	object3d_manager.update();
	shader_manager.update();
	camera.update();

	//camera animation
	const float *mp = window.getMouse_move();
	const float mw = window.getMouse_wheel();
	const bool* mb = window.getMouse_Button();
	if (mb[1]) {
		camera.TranslateFunc(mp[0], mp[1]);
	}
	if (mb[2]) {
		camera.RotateFunc(mp[0], mp[1]);
	}
	camera.ZoomFunc(mw);

	//guiの表示
	if (showgui) { showGUI(); }

	//swapbufferの起動
	window.swapBuffers();
	_glError(__FILE__, __LINE__);
}

void HR::shutdown()
{
	texture_manager.shutdownTexture();
	material_manager.shutdownMaterial();
	light_manager.shutdownLight();
	object3d_manager.shutdownBasic3D();
	shader_manager.shutdownShader();
	window.shutdown();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	_glError(__FILE__, __LINE__);
}

void HR::object_move(Basic3D& obj)
{
	const bool* mb = window.getMouse_Button();
	obj.setForce(0.0, 0.0, 0.0);
	if (mb[0]) {
		const float *mp = window.getMouse_move();
		float pos[3];
		float dir[3] = { camera.getDirection()[0],camera.getDirection()[1],camera.getDirection()[2] };
		mouseTranslate(mp[0], mp[1], dir, pos);
		obj.addPosition(pos[0], pos[1], pos[2]);
	}
}

void HR::ObjectSetting()
{
	ImGui::Begin("Object Setting");
	//light_manager.setting();
	//basic3d_manager.setting();
	//texture_manager.setting();
	//material_manager.setting();
	ImGui::End();
}

void HR::showResource()
{
	ImGui::Begin("Resource");
	if (ImGui::TreeNode("camera")) {
		float pos[3];
		deepcopy_vec3(camera.getPosition(), pos);
		ImGui::SliderFloat3("position", pos, -20.0f, 20.0f);
		camera.setPosition(pos[0], pos[1], pos[2]);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("skybox")) {
		float fovy = skybox.getFovy();
		ImGui::SliderFloat("fovy", &fovy, 29.0f, 31.0f);
		skybox.setFovy(fovy);
		ImGui::TreePop();
	}
	light_manager.showResource();
	object3d_manager.showResource();
	texture_manager.showResource();
	material_manager.showResource();
	shader_manager.showResource();
	ImGui::End();
}

void HR::showGUI() {

	// Start the ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	showResource();
	//ObjectSetting();

	{
		ImGui::Begin("Framerate");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}


	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwMakeContextCurrent(window.getWindow());
	glfwGetFramebufferSize(window.getWindow(), &display_w, &display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

bool HR::shouldClose()
{
	//skyboxの表示
	bool frag = window.shouldClose();
	//shader_manager.getShader<SkyboxShader>("sbs").draw(skybox, camera);
	return frag;
}