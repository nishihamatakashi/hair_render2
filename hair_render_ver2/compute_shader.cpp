#include "compute_shader.h"
#define IX(i,j,k) ((i) + (j) * (N) + (k) * (N) * (N))

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
	loadUnilocate(0, 11, "fluidpos", comp0);
	loadUnilocate(0, 12, "fluidsize", comp0);
	loadUnilocate(0, 13, "hairpos", comp0);
	loadUnilocate(0, 14, "fluid_dens", comp0);
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

void HairFTLShader::setUniform(Basic3D &object, int programID, Fluid3D& fluid)
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
		glUniform3fv(loc0[11], 1, fluid.getPosition());
		glUniform3fv(loc0[12], 1, fluid.getScale());
		glUniform3fv(loc0[13], 1, object.getPosition());

		glActiveTexture(GL_TEXTURE0);
		glBindImageTexture(0, object.getTbo()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE1);
		glBindImageTexture(1, object.getTbo()[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE2);
		glBindImageTexture(2, object.getTbo()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glActiveTexture(GL_TEXTURE3);
		glBindImageTexture(3, object.space_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glUniform1i(loc0[14], 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_3D, fluid.getSimulate_tex()[2]);

		glActiveTexture(GL_TEXTURE5);
		glBindImageTexture(5, fluid.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
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

void HairFTLShader::update(Basic3D& object,Fluid3D& fluid)
{
	//ƒƒ‚ƒŠ‚ª‘‚¦‚é‚æ[
	glTextureSubImage3D(object.space_tex, 0, 0, 0, 0, GLsizei(object.getParameter()[4]), GLsizei(object.getParameter()[4]), GLsizei(object.getParameter()[4]), GL_RGBA, GL_FLOAT, &object.v[0]);
	glUseProgram(compute_shaders.at(0)->getProgram());
	setUniform(object, 0,fluid);
	glDispatchCompute(object.getResolution()[0], object.getResolution()[1], 1);
	_glError(__FILE__, __LINE__);

	glUseProgram(compute_shaders.at(1)->getProgram());
	setUniform(object, 1,fluid);
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

void FluidShader::createComputeShader(
	const char *comp0, const char *comp1,
	const char *comp2, const char *comp3,
	const char *comp4, const char *comp5,
	const char *comp6) 
{
	const char* comps[7] = { comp0,comp1,comp2,comp3,comp4,comp5,comp6 };
	for (int i = 0; i <=6;i++) 
	{
		addComputeShader(comps[i]);
		loadUnilocate(i, 0, "visc", comps[i]);
		loadUnilocate(i, 1, "diff", comps[i]);
		loadUnilocate(i, 2, "resolution", comps[i]);
		loadUnilocate(i, 3, "dt", comps[i]);
		loadUnilocate(i, 4, "b", comps[i]);
		loadUnilocate(i, 5, "pressureflag", comps[i]);
		loadUnilocate(i, 6, "vdiv", comps[i]);
		_glError(__FILE__, __LINE__);
	}
}
void FluidShader::setUniform(Fluid3D &object, UPDATEFLAG f, int programID, int b, float dt)
{
	GLint *loc0 = compute_shaders.at(programID)->getUnilocate();
	glUniform1f(loc0[0], object.getParameter()[0]);
	glUniform1f(loc0[1], object.getParameter()[1]);
	glUniform1i(loc0[2], object.getCell_size());
	glUniform1f(loc0[3], dt);
	glUniform1i(loc0[4], b);
	glUniform1i(loc0[5], int(object.getCompress_flag()));
	
	float a = 0.0f;
	object.y_volume += a * dt;
	float kp = 2.3f * (25.0f * dt);
	float ki = kp * kp / 16.0f;
	float vdiv = 0.005f * (-(kp*a + ki * object.y_volume) / (a + 1.0f));

	glUniform1f(loc0[6], vdiv);

	if (f == Density)
	{
		if (programID == 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[3], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//diffuse‚ÌŽž‚ÌÝ’è
		else if (programID == 1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[3], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//advect‚ÌŽž‚ÌÝ’è
		else if (programID == 2)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE2);
			glBindImageTexture(2, object.getSimulate_tex()[3], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//setbnd‚ÌŽž‚ÌÝ’è
		else if (programID == 6)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
	}
	else if (Velocity) 
	{
		//add_data‚ÌŽž‚ÌÝ’è
		if (programID == 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE2);
			glBindImageTexture(2, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//diffuse‚ÌŽž‚ÌÝ’è
		else if (programID == 1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//advect‚ÌŽž‚ÌÝ’è
		else if (programID == 2)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE2);
			glBindImageTexture(2, object.getSimulate_tex()[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//divergence‚ÌŽž‚ÌÝ’è
		else if (programID == 3)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[4], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE2);
			glBindImageTexture(2, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//project‚ÌŽž‚ÌÝ’è
		else if (programID == 4)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[4], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[5], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE2);
			glBindImageTexture(2, object.getSimulate_tex()[2], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//projvel‚ÌŽž‚ÌÝ’è
		else if (programID == 5)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			glActiveTexture(GL_TEXTURE1);
			glBindImageTexture(1, object.getSimulate_tex()[5], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		}
		//set_bnd‚ÌŽž‚ÌÝ’è
		else if (programID == 6)
		{
			if (b == 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindImageTexture(0, object.getSimulate_tex()[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

			}
			else if (b == 1)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindImageTexture(0, object.getSimulate_tex()[4], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			}
			else if (b == 2)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindImageTexture(0, object.getSimulate_tex()[5], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			}
		}
	}
}

void FluidShader::dispatch(Fluid3D &object, UPDATEFLAG f, int  programID, int b, float dt)
{
	glUseProgram(compute_shaders.at(programID)->getProgram());
	setUniform(object,f,programID, b, dt);
	GLint size = object.getCell_size();
	glDispatchCompute(size, size, size);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void FluidShader::updateVel(Fluid3D& object, const float dt)
{
	dispatch(object, Velocity ,0, 0, dt);	//add_datga
	dispatch(object, Velocity, 6, 0, dt);	//set_bnd(vel)
	swap(object,Velocity);					//swap


	for (int i = 0; i < LINEARSOLVERTIMES; i++) {
		dispatch(object, Velocity, 1, 0, dt);	//diffuse
		dispatch(object, Velocity, 6, 0, dt);	//set_bnd(vel)
	}
	swap(object, Velocity);				//swap


	dispatch(object, Velocity, 3, 0, dt);	//divergence
	dispatch(object, Velocity, 6, 1, dt);	//set_bnd(div)
	dispatch(object, Velocity, 6, 2, dt);	//set_bnd(press)


	for (int i = 0; i < LINEARSOLVERTIMES; i++) {
		dispatch(object, Velocity, 4, 0, dt);	//project
		dispatch(object, Velocity, 6, 2, dt);	//set_bnd(press)
	}

	dispatch(object, Velocity, 5, 0, dt);	//projvel
	dispatch(object, Velocity, 6, 0, dt);	//set_bnd(vel)

	swap(object, Velocity);					//swap

	dispatch(object, Velocity, 2, 0, dt);	//advect
	dispatch(object, Velocity, 6, 0, dt);	//set_bnd(vel)

	dispatch(object, Velocity, 3, 0, dt);	//divergence
	dispatch(object, Velocity, 6, 1, dt);	//set_bnd(div)
	dispatch(object, Velocity, 6, 2, dt);	//set_bnd(press)

	for (int i = 0; i < LINEARSOLVERTIMES; i++) {
		dispatch(object, Velocity, 4, 0, dt);	//project
		dispatch(object, Velocity, 6, 2, dt);	//set_bnd(press)
	}

	dispatch(object, Velocity, 5, 0, dt);	//projvel
	dispatch(object, Velocity, 6, 0, dt);	//set_bnd(vel)
}

void FluidShader::updateDens(Fluid3D& object, const float dt)
{
	dispatch(object, Density, 0, 1, dt);	//add_data
	dispatch(object, Density, 6, 1, dt);	//set_bnd	
	swap(object, Density);			//swap

	for (int i = 0; i < LINEARSOLVERTIMES; i++) {
		dispatch(object, Density, 1, 1, dt);	//diffuse
		dispatch(object, Density, 6, 1, dt);	//set_bnd
	}

	swap(object, Density);				//swap	

	dispatch(object, Density, 2, 1, dt);	//advect
	dispatch(object, Density, 6, 1, dt);	//set_bnd
}

void FluidShader::update(Fluid3D& object, const float dt)
{
	updateVel(object, dt);
	updateDens(object, dt);
}

void FluidShader::add_data(Fluid3D &data)
{
	int N = data.getCell_size();
	std::vector<GLfloat> &d = data.getAdd_dens();
	std::vector<GLfloat> &v = data.getAdd_vel();

	for (int num = 0; num< (N * N * N); num++)
	{
		int i = num / (N * N);
		int j = (num - (i * N * N)) / N;
		int k = (num - (i * N * N)) % N;
		int half_n = N * 0.5f;
		if (j > half_n - 2 && j < half_n + 2 &&  i == 2 && k > half_n - 2 && k < half_n + 2) {
			d.at(IX(i, j, k) * 4) = 1000.0f;
			d.at(IX(i, j, k) * 4 + 1) = 0.0f;
			d.at(IX(i, j, k) * 4 + 2) = 0.0f;
			d.at(IX(i, j, k) * 4 + 3) = 0.0f;

			v.at(IX(i, j, k) * 4) = 1000.0f;
			v.at(IX(i, j, k) * 4 + 1) = 0.0f;
			v.at(IX(i, j, k) * 4 + 2) = 0.0f;
			v.at(IX(i, j, k) * 4 + 3) = 0.0f;
		}
	}
	//ƒƒ‚ƒŠ‚ª‘‚¦‚é‚æ[
	glTextureSubImage3D(data.getSimulate_tex()[3], 0, 0, 0, 0, N, N, N, GL_RGBA, GL_FLOAT, &d.at(0));
	glTextureSubImage3D(data.getSimulate_tex()[1], 0, 0, 0, 0, N, N, N, GL_RGBA, GL_FLOAT, &v.at(0));
}