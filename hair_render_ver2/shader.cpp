#include"shader.h"

//�V�F�[�_�I�u�W�F�N�g�̃R���p�C��
//shader:�V�F�[�_�I�u�W�F�N�g��
//str	:�R���p�C���G���[�����������v���O����
GLboolean printShaderInfoLog(GLuint shader, const char* name)
{
	//�R���p�C�����ʂ��擾
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Compile Error in " << name << std::endl;
	}

	//�G���[���O�̒������i�[����
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize>1)
	{
		//�V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
		std::vector<GLchar>infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//�v���O�����I�u�W�F�N�g�̃����N���ʂ�\������
//	program:	�v���O�����I�u�W�F�g��
GLboolean printProgramInfoLog(GLuint program)
{
	//�����N���ʂ��擾����
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link error" << std::endl;

	//�V�F�[�_�̃����N���̃��O�̒������擾����
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1)
	{
		//�V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����B
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//�V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
//	name; �V�F�[�_�[�̃\�[�X�t�@�C����
GLchar *readShaderSource(const char* name)
{
	//�t�@�C������NULL�Ȃ�NULL��Ԃ��B
	if (name == NULL) return NULL;

	//�\�[�X�t�@�C�����J��
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		//�J���Ȃ�����
		std::cerr << "Error: Can't open source file:" << name << std::endl;
		return NULL;
	}

	//�t�@�C���̖����Ɉړ������݈ʒu(=�t�@�C���T�C�Y)�𓾂�
	file.seekg(0L, std::ios::end);
	const GLsizei length(static_cast<GLsizei>(file.tellg()));

	//�t�@�C���T�C�Y+������̏I�[����("\0")���̃��������m��
	GLchar *buffer(new(std::nothrow)GLchar[length + 1]);
	if (buffer == NULL)
	{
		//�������s��
		std::cerr << "Error; Too large file" << name << std::endl;
		file.close();
		return NULL;
	}

	//�t�@�C����擪����ǂ�
	file.seekg(0L, std::ios::beg);
	file.read(buffer, length);
	buffer[length] = '\0';

	if (file.bad())
	{
		//��肭�ǂݍ��߂Ȃ�����
		std::cerr << "Error; Could not read source file : " << name << std::endl;
		delete[] buffer;
		buffer = NULL;
	}
	return buffer;
}

GLuint createDrawShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	// �V�F�[�_�v���O�����̍쐬
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
			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
			const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
			glShaderSource(vobj, 1, &vsrc, nullptr);
			glCompileShader(vobj);

			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
			if (printShaderInfoLog(vobj, vert)) glAttachShader(program, vobj);
			glDeleteShader(vobj);
		}

		if (fsrc != NULL) {

			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
			const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(fobj, 1, &fsrc, NULL);
			glCompileShader(fobj);

			//vertex shader���v���O�����I�u�W�F�N�g�Ƀ����N����
			if (printShaderInfoLog(fobj, frag)) glAttachShader(program, fobj);
			glDeleteShader(fobj);
		}

		if (gsrc != NULL)
		{
			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
			const GLuint gobj(glCreateShader(GL_GEOMETRY_SHADER));
			glShaderSource(gobj, 1, &gsrc, nullptr);
			glCompileShader(gobj);

			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
			if (printShaderInfoLog(gobj, geom)) glAttachShader(program, gobj);
			glDeleteShader(gobj);
		}

		if (tcsrc != NULL) {

			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
			const GLuint tcobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(tcobj, 1, &tcsrc, NULL);
			glCompileShader(tcobj);

			//vertex shader���v���O�����I�u�W�F�N�g�Ƀ����N����
			if (printShaderInfoLog(tcobj, tessc)) glAttachShader(program, tcobj);
			glDeleteShader(tcobj);
		}

		if (tesrc != NULL)
		{
			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
			const GLuint teobj(glCreateShader(GL_VERTEX_SHADER));
			glShaderSource(teobj, 1, &tesrc, nullptr);
			glCompileShader(teobj);

			// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
			if (printShaderInfoLog(teobj, tesse)) glAttachShader(program, teobj);
			glDeleteShader(teobj);
		}
	}

	// �v���O�����I�u�W�F�N�g�������N����   
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

	// �v���O�������쐬�ł��Ȃ����0��Ԃ�
	glDeleteProgram(program);

	return 0;
}

GLuint createComputeShader(const char* code)
{
	const GLchar *csrc(readShaderSource(code));

	//����̃v���O�����I�u�W�F�N�g�𐶐�����
	const GLuint program(glCreateProgram());

	//compute shader���v���O�����I�u�W�F�N�g�Ƀ����N
	if (csrc != NULL) {
		const GLuint cobj(glCreateShader(GL_COMPUTE_SHADER));
		glShaderSource(cobj, 1, &csrc, NULL);
		glCompileShader(cobj);

		//compute shader���v���O�����I�u�W�F�N�g�Ƀ����N����
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

	// �v���O�������쐬�ł��Ȃ����0��Ԃ�
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