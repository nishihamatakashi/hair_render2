#include"shader.h"

//シェーダオブジェクトのコンパイル
//shader:シェーダオブジェクト名
//str	:コンパイルエラーが発生したプログラム
GLboolean printShaderInfoLog(GLuint shader, const char* name)
{
	//コンパイル結果を取得
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Compile Error in " << name << std::endl;
	}

	//エラーログの長さを格納する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize>1)
	{
		//シェーダのコンパイル時のログの内容を取得する
		std::vector<GLchar>infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//プログラムオブジェクトのリンク結果を表示する
//	program:	プログラムオブジェト名
GLboolean printProgramInfoLog(GLuint program)
{
	//リンク結果を取得する
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link error" << std::endl;

	//シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1)
	{
		//シェーダのコンパイル時のログの内容を取得する。
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//シェーダのソースファイルを読み込んだメモリを返す
//	name; シェーダーのソースファイル名
GLchar *readShaderSource(const char* name)
{
	//ファイル名がNULLならNULLを返す。
	if (name == NULL) return NULL;

	//ソースファイルを開く
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		//開けなかった
		std::cerr << "Error: Can't open source file:" << name << std::endl;
		return NULL;
	}

	//ファイルの末尾に移動し現在位置(=ファイルサイズ)を得る
	file.seekg(0L, std::ios::end);
	const GLsizei length(static_cast<GLsizei>(file.tellg()));

	//ファイルサイズ+文字列の終端文字("\0")分のメモリを確保
	GLchar *buffer(new(std::nothrow)GLchar[length + 1]);
	if (buffer == NULL)
	{
		//メモリ不足
		std::cerr << "Error; Too large file" << name << std::endl;
		file.close();
		return NULL;
	}

	//ファイルを先頭から読む
	file.seekg(0L, std::ios::beg);
	file.read(buffer, length);
	buffer[length] = '\0';

	if (file.bad())
	{
		//上手く読み込めなかった
		std::cerr << "Error; Could not read source file : " << name << std::endl;
		delete[] buffer;
		buffer = NULL;
	}
	return buffer;
}

GLuint createDrawShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	// シェーダプログラムの作成
	const GLuint program(glCreateProgram());


	GLchar* vsrc = readShaderSource(vert);
	GLchar* fsrc = readShaderSource(frag);
	GLchar* gsrc = geom == 0 ? NULL : readShaderSource(geom);
	GLchar* tesrc = tessc == 0 ? NULL : readShaderSource(tessc);
	GLchar* tcsrc = tesse == 0 ? NULL : readShaderSource(tesse);

	if (program > 0)
	{
		if (vsrc != NULL)
		{
			// バーテックスシェーダのシェーダオブジェクトを作成する
			const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
			glShaderSource(vobj, 1, &vsrc, nullptr);
			glCompileShader(vobj);

			// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
			if (printShaderInfoLog(vobj, vert)) glAttachShader(program, vobj);
			glDeleteShader(vobj);
		}

		if (fsrc != NULL) {

			// バーテックスシェーダのシェーダオブジェクトを作成する
			const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(fobj, 1, &fsrc, NULL);
			glCompileShader(fobj);

			//vertex shaderをプログラムオブジェクトにリンクする
			if (printShaderInfoLog(fobj, frag)) glAttachShader(program, fobj);
			glDeleteShader(fobj);
		}

		if (gsrc != NULL)
		{
			// バーテックスシェーダのシェーダオブジェクトを作成する
			const GLuint gobj(glCreateShader(GL_GEOMETRY_SHADER));
			glShaderSource(gobj, 1, &gsrc, nullptr);
			glCompileShader(gobj);

			// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
			if (printShaderInfoLog(gobj, geom)) glAttachShader(program, gobj);
			glDeleteShader(gobj);
		}

		if (tcsrc != NULL) {

			// バーテックスシェーダのシェーダオブジェクトを作成する
			const GLuint tcobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(tcobj, 1, &tcsrc, NULL);
			glCompileShader(tcobj);

			//vertex shaderをプログラムオブジェクトにリンクする
			if (printShaderInfoLog(tcobj, tessc)) glAttachShader(program, tcobj);
			glDeleteShader(tcobj);
		}

		if (tesrc != NULL)
		{
			// バーテックスシェーダのシェーダオブジェクトを作成する
			const GLuint teobj(glCreateShader(GL_VERTEX_SHADER));
			glShaderSource(teobj, 1, &tesrc, nullptr);
			glCompileShader(teobj);

			// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
			if (printShaderInfoLog(teobj, tesse)) glAttachShader(program, teobj);
			glDeleteShader(teobj);
		}
	}

	// プログラムオブジェクトをリンクする   
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindAttribLocation(program, 2, "texcoord");
	glBindAttribLocation(program, 3, "bone_index");
	glBindAttribLocation(program, 4, "bone_weight");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	delete vsrc;
	delete fsrc;
	delete gsrc;
	delete tcsrc;
	delete tesrc;

	if (printProgramInfoLog(program)) {
		return program;
	}
	else {
		std::cerr << "Can't create program" << std::endl;
	}

	// プログラムが作成できなければ0を返す
	glDeleteProgram(program);

	return 0;
}

GLuint createComputeShader(const char* code)
{
	const GLchar *csrc(readShaderSource(code));

	//からのプログラムオブジェクトを生成する
	const GLuint program(glCreateProgram());

	//compute shaderをプログラムオブジェクトにリンク
	if (csrc != NULL) {
		const GLuint cobj(glCreateShader(GL_COMPUTE_SHADER));
		glShaderSource(cobj, 1, &csrc, NULL);
		glCompileShader(cobj);

		//compute shaderをプログラムオブジェクトにリンクする
		if (printShaderInfoLog(cobj, code)) {
			glAttachShader(program, cobj);
		}
		glDeleteShader(cobj);
	}
	glLinkProgram(program);
	delete csrc;

	if (printProgramInfoLog(program)) {
		return program;
	}

	// プログラムが作成できなければ0を返す
	glDeleteProgram(program);

	return 0;

}

void Shader::shutdown() 
{ 
	glDeleteProgram(this->program); 
}
void Shader::loadUnilocate(size_t n, const char *name, const char *code)
{
	unilocate[n] = glGetUniformLocation(program, name);
	//ErrorUnilocate(unilocate[n], name, code);
}
GLuint Shader::getProgram()
{ 
	return this->program; 
}
GLint* Shader::getUnilocate()
{ 
	return unilocate; 
}

void Shader::loadShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	program = createDrawShader(vert, frag, geom, tessc, tesse);
	loadUnilocate(0, "model", vert);
	loadUnilocate(1, "view", vert);
	loadUnilocate(2, "proj", vert);
}

void Shader::loadShader(const char* comp)
{
	program = createComputeShader(comp);
}