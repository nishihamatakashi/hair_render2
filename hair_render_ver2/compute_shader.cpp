#include "compute_shader.h"

void ComputeShader::addComputeShader(const char* comp)
{
	Shader* e = new Shader();
	e->loadShader(comp);
	std::unique_ptr<Shader>uPtr{ e };
	compute_shaders.emplace_back(std::move(uPtr));
}

void ComputeShader::loadUnilocate(size_t programID, size_t unilocateID, const char *name, const char *code)
{
	bool check = compute_shaders.size() > programID ? true : false;
	assert(check && "ErrorFcompute shader size is over");
	compute_shaders.at(programID)->loadUnilocate(unilocateID, name, code);
}

void ComputeShader::shutdown()
{
	for (int i = 0; compute_shaders.size(); i++)
	{
		compute_shaders.at(i)->shutdown();
	}
}

void TestComputeShader::createComputeShader(const char *comp)
{
	addComputeShader(comp);
	_glError(__FILE__, __LINE__);
	loadUnilocate(0, 0, "resolution", comp);
	loadUnilocate(0, 1, "m", comp);
}

void TestComputeShader::setUniform(Basic3D &object)
{
	GLint *loc0 = compute_shaders.at(0)->getUnilocate();
	glUniform2iv(loc0[0], 1, object.getResolution());
	glUniform1f(loc0[1], 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindImageTexture(0, object.getTbo()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}
void TestComputeShader::update(Basic3D& object)
{
	glUseProgram(compute_shaders.at(0)->getProgram());
	setUniform(object);
	glDispatchCompute(object.getResolution()[0], object.getResolution()[1], 1);
	_glError(__FILE__, __LINE__);
}

void HairFTLShader::createComputeShader(const char *comp0, const char *comp1)
{
	addComputeShader(comp0);
	loadUnilocate(0, 0, "knots", comp0);
	loadUnilocate(0, 1, "hlength", comp0);
	loadUnilocate(0, 2, "radius", comp0);
	loadUnilocate(0, 3, "resolution", comp0);
	loadUnilocate(0, 4, "cnum", comp0);
	loadUnilocate(0, 5, "clen", comp0);
	loadUnilocate(0, 6, "hardness1", comp0);
	loadUnilocate(0, 7, "hardness2", comp0);
	loadUnilocate(0, 8, "adhesion", comp0);
	loadUnilocate(0, 9, "type", comp0);
	loadUnilocate(0, 10, "move_force", comp0);
	loadUnilocate(0, 11, "spherepos", comp0);
	_glError(__FILE__, __LINE__);


	addComputeShader(comp1);
	loadUnilocate(1, 0, "knots", comp1);
	loadUnilocate(1, 1, "hlength", comp1);
	loadUnilocate(1, 2, "radius", comp1);
	loadUnilocate(1, 3, "resolution", comp1);
	loadUnilocate(1, 4, "cnum", comp1);
	loadUnilocate(1, 5, "clen", comp1);
	loadUnilocate(1, 6, "adhesion", comp1);
	loadUnilocate(1, 7, "space", comp1);
	_glError(__FILE__, __LINE__);
}

void HairFTLShader::setUniform(Basic3D &object, int programID,const float *p)
{
	GLint *loc0 = compute_shaders.at(programID)->getUnilocate();

	if (programID == 0) {
		glUniform1f(loc0[0], object.getParameter()[1]);
		glUniform1f(loc0[1], object.getParameter()[2]);
		glUniform1f(loc0[2], object.getParameter()[3]);
		glUniform2iv(loc0[3], 1, object.getResolution());
		glUniform1f(loc0[4], object.getParameter()[4]);
		glUniform1f(loc0[5], object.getParameter()[5]);
		glUniform1f(loc0[6], object.getParameter()[6]);
		glUniform1f(loc0[7], object.getParameter()[7]);
		glUniform1f(loc0[8], object.getParameter()[8]);
		glUniform1i(loc0[9], GLint(object.getType()));
		glUniform3fv(loc0[10], 1, object.getForce());
		glUniform3fv(loc0[11], 1, p);

		glActiveTexture(GL_TEXTURE0);
		glBindImageTexture(0, object.getTbo()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE1);
		glBindImageTexture(1, object.getTbo()[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE2);
		glBindImageTexture(2, object.getTbo()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE3);
		glBindImageTexture(3, object.space_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}
	else if (programID == 1)
	{
		glUniform1f(loc0[0], object.getParameter()[1]);
		glUniform1f(loc0[1], object.getParameter()[2]);
		glUniform1f(loc0[2], object.getParameter()[3]);
		glUniform2iv(loc0[3], 1, object.getResolution());
		glUniform1f(loc0[4], object.getParameter()[4]);
		glUniform1f(loc0[5], object.getParameter()[5]);
		glUniform1f(loc0[6], object.getParameter()[8]);

		glActiveTexture(GL_TEXTURE0);
		glBindImageTexture(0, object.getTbo()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE1);
		glBindImageTexture(1, object.getTbo()[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE2);
		glBindImageTexture(2, object.getTbo()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


		glUniform1i(loc0[7], 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, object.space_tex);

	}
}

void HairFTLShader::update(Basic3D& object,const float *p)
{
	//ƒƒ‚ƒŠ‚ª‘‚¦‚é‚æ[
	glTextureSubImage3D(object.space_tex, 0, 0, 0, 0, GLsizei(object.getParameter()[4]), GLsizei(object.getParameter()[4]), GLsizei(object.getParameter()[4]), GL_RGBA, GL_FLOAT, &object.v[0]);
	glUseProgram(compute_shaders.at(0)->getProgram());
	setUniform(object, 0,p);
	glDispatchCompute(object.getResolution()[0], object.getResolution()[1], 1);
	_glError(__FILE__, __LINE__);

	glUseProgram(compute_shaders.at(1)->getProgram());
	setUniform(object, 1,p);
	glDispatchCompute(object.getResolution()[0], object.getResolution()[1], 1);
	_glError(__FILE__, __LINE__);
}

void BitnicSortShader::createComputeShader(const char *comp0)
{
	addComputeShader(comp0);
	loadUnilocate(0, 0, "resolution", comp0);
	loadUnilocate(0, 1, "block", comp0);
	loadUnilocate(0, 2, "cstep", comp0);
	loadUnilocate(0, 3, "offset", comp0);
	_glError(__FILE__, __LINE__);
}

void BitnicSortShader::setUniform(GLuint tex0, GLuint tex1, int progamID, int block, int step, int offset, const int *size)
{
	float fsize[2] = { float(size[0]),float(size[1]) };
	GLint *loc0 = compute_shaders.at(0)->getUnilocate();
	glUniform2fv(loc0[0], 1, fsize);
	glUniform1f(loc0[1], float(block));
	glUniform1f(loc0[2], float(step));
	glUniform1f(loc0[3], float(offset));

	glActiveTexture(GL_TEXTURE0);
	glBindImageTexture(0, tex0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	glActiveTexture(GL_TEXTURE1);
	glBindImageTexture(1, tex1, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void BitnicSortShader::update(GLuint tex0, GLuint tex1, const int* size)
{
	for (int block = 2; block <= (size[0] * size[1]); block *= 2) {
		for (int step = block; step >= 2; step /= 2) {
			int offset = step / 2;
			glUseProgram(compute_shaders.at(0)->getProgram());
			setUniform(tex0, tex1, 0, block, step, offset, size);
			glDispatchCompute(size[0], size[1], 1);

			GLuint tmp = tex0;
			tex0 = tex1;
			tex1 = tmp;
		}
	}
	_glError(__FILE__, __LINE__);
}