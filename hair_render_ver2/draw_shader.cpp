#include"draw_shader.h"

void PointShader::createShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	loadShader(vert, frag, geom, tessc, tesse);
	loadUnilocate(3, "color", frag);
	loadUnilocate(4, "tex_position", vert);
}

void PointShader::setUniform(Basic3D &object, Light &light, Material &material)
{
	glUniformMatrix4fv(unilocate[0], 1, GL_FALSE, object.getMatrix()->model);
	glUniformMatrix4fv(unilocate[1], 1, GL_FALSE, object.getMatrix()->view);
	glUniformMatrix4fv(unilocate[2], 1, GL_FALSE, object.getMatrix()->proj);
	glUniform3fv(unilocate[3], 1, material.getcolor());

	glUniform1i(unilocate[4], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, object.getTbo()[0]);
}
void PointShader::draw(Basic3D& object, Light& light, Camera& camera)
{
	checkDrawObject<Basic3D>(object);
	object.setEnableFunc();
	object.setMatrix(camera);
	glUseProgram(this->getProgram());
	this->setUniform(object, light, *object.getMaterial());
	glBindVertexArray(object.getVao());
	glPointSize(10.0f);
	glDrawArrays(GL_POINTS, 0, object.getVertices());
	glBindVertexArray(0);
	_glError(__FILE__, __LINE__);
}

void LineShader::createShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	loadShader(vert, frag, geom, tessc, tesse);

	loadUnilocate(3, "color", frag);
	loadUnilocate(4, "tex_position", vert);
}
void LineShader::setUniform(Basic3D &object, Light &light, Material &material)
{
	glUniformMatrix4fv(unilocate[0], 1, GL_FALSE, object.getMatrix()->model);
	glUniformMatrix4fv(unilocate[1], 1, GL_FALSE, object.getMatrix()->view);
	glUniformMatrix4fv(unilocate[2], 1, GL_FALSE, object.getMatrix()->proj);
	glUniform3fv(unilocate[3], 1, material.getcolor());

	glUniform1i(unilocate[4], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, object.getTbo()[0]);
}
void LineShader::draw(Basic3D& object, Light& light, Camera& camera)
{
	checkDrawObject<Basic3D>(object);
	object.setEnableFunc();
	object.setMatrix(camera);
	glUseProgram(this->getProgram());
	this->setUniform(object, light, *object.getMaterial());
	glBindVertexArray(object.getVao());
	glDrawElements(GL_LINE_LOOP, sizeof(GLuint[3]) * object.getIndices(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshShader::createShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	loadShader(vert, frag, geom, tessc, tesse);
	_glError(__FILE__, __LINE__);

	loadUnilocate(3, "adjoint", vert);

	loadUnilocate(4, "kdiff", frag);
	loadUnilocate(5, "kspec", frag);
	loadUnilocate(6, "kamb", frag);
	loadUnilocate(7, "kshi", frag);

	loadUnilocate(8, "lpos", vert);
	loadUnilocate(9, "lcolor", frag);
	loadUnilocate(10, "lvector", frag);
	loadUnilocate(11, "lsize", frag);
	loadUnilocate(12, "lenergy", frag);
	loadUnilocate(13, "ldistance", frag);
	loadUnilocate(14, "lspot_size", frag);
	loadUnilocate(15, "lblend", frag);
	loadUnilocate(16, "lgamma", frag);
	loadUnilocate(17, "ltype", frag);

	loadUnilocate(18, "dmap", frag);
	loadUnilocate(19, "nmap", frag);
	loadUnilocate(20, "smap", frag);
	loadUnilocate(21, "cubemap", frag);
	loadUnilocate(22, "texFlag", frag);
	loadUnilocate(23, "roughness", frag);
	loadUnilocate(24, "transparency", frag);
}
void MeshShader::setUniform(Basic3D &object, Light &light, Material &material)
{
	glUniformMatrix4fv(unilocate[0], 1, GL_FALSE, object.getMatrix()->model);
	glUniformMatrix4fv(unilocate[1], 1, GL_FALSE, object.getMatrix()->view);
	glUniformMatrix4fv(unilocate[2], 1, GL_FALSE, object.getMatrix()->proj);
	glUniformMatrix4fv(unilocate[3], 1, GL_FALSE, object.getMatrix()->adjoint);

	glUniform3fv(unilocate[4], 1, material.getKDiff());
	glUniform3fv(unilocate[5], 1, material.getKSpec());
	glUniform3fv(unilocate[6], 1, material.getKAmb());

	glUniform1f(unilocate[7], material.getKshi());
	glUniform1f(unilocate[23], material.getRoughness());
	glUniform1f(unilocate[24], material.getTransparency());

	glUniform3fv(unilocate[8], 1, light.getPosition());
	glUniform3fv(unilocate[9], 1, light.getColor());
	glUniform3fv(unilocate[10], 1, light.getVector());
	glUniform2fv(unilocate[11], 1, light.getSize());
	glUniform1f(unilocate[12], light.getEnergy());
	glUniform1f(unilocate[13], light.getDistance());
	glUniform1f(unilocate[14], light.getSpot_size());
	glUniform1f(unilocate[15], light.getBlend());
	glUniform1f(unilocate[16], light.getGamma());
	glUniform1i(unilocate[17], light.getType());

	glUniform1i(unilocate[18], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.getAlbedoMap());

	glUniform1i(unilocate[19], 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.getAlbedoMap());

	glUniform1i(unilocate[20], 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material.getAlbedoMap());

	glUniform1i(unilocate[21], 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, material.getCubeMap());

	glUniformMatrix4fv(unilocate[22], 1, GL_FALSE, material.getTex_Flag());
}

void MeshShader::draw(Basic3D& object, Light& light, Camera& camera)
{
	checkDrawObject<Basic3D>(object);
	object.setEnableFunc();
	object.setMatrix(camera);
	glUseProgram(this->getProgram());
	this->setUniform(object, light, *object.getMaterial());
	glBindVertexArray(object.getVao());
	glDrawElements(GL_TRIANGLES, sizeof(GLuint[3]) * object.getIndices(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	_glError(__FILE__, __LINE__);
}


void HairShader::createShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	loadShader(vert, frag, geom, tessc, tesse);
	_glError(__FILE__, __LINE__);

	loadUnilocate(3, "adjoint", vert);

	//ÞŽ¿
	loadUnilocate(4, "kdiff", frag);
	loadUnilocate(5, "kspec", frag);
	loadUnilocate(6, "kamb", frag);
	loadUnilocate(7, "kshi", frag);

	//light î•ñ
	loadUnilocate(8, "lpos", vert);
	loadUnilocate(9, "lcolor", frag);
	loadUnilocate(10, "lvector", frag);
	loadUnilocate(11, "lsize", frag);
	loadUnilocate(12, "lenergy", frag);
	loadUnilocate(13, "ldistance", frag);
	loadUnilocate(14, "lspot_size", frag);
	loadUnilocate(15, "lblend", frag);
	loadUnilocate(16, "lgamma", frag);
	loadUnilocate(17, "ltype", frag);

	//textures
	loadUnilocate(18, "dmap", frag);
	loadUnilocate(19, "nmap", frag);
	loadUnilocate(20, "smap", frag);
	loadUnilocate(21, "cubemap", frag);
	loadUnilocate(22, "texFlag", frag);

	//hair_parameter
	loadUnilocate(23, "knots", vert);
	loadUnilocate(24, "depthmap", frag);
	loadUnilocate(25, "weight", geom);
	loadUnilocate(26, "beta_n", frag);
	loadUnilocate(27, "beta_m", frag);
	loadUnilocate(28, "alpha", frag);
	loadUnilocate(29, "sigma", frag);
	loadUnilocate(30, "corners", geom);
	loadUnilocate(31, "sharp", geom);
	loadUnilocate(32, "hlength", geom);
	loadUnilocate(33, "roughness", frag);
	loadUnilocate(34, "transparency", frag);
	loadUnilocate(35, "wetflag", frag);
	loadUnilocate(36, "alpha_dec", frag);
}

void HairShader::setUniform(Basic3D &object, Light &light, Material &material)
{
	glUniformMatrix4fv(unilocate[0], 1, GL_FALSE, object.getMatrix()->model);
	glUniformMatrix4fv(unilocate[1], 1, GL_FALSE, object.getMatrix()->view);
	glUniformMatrix4fv(unilocate[2], 1, GL_FALSE, object.getMatrix()->proj);
	glUniformMatrix4fv(unilocate[3], 1, GL_FALSE, object.getMatrix()->adjoint);

	glUniform3fv(unilocate[4], 1, material.getKDiff());
	glUniform3fv(unilocate[5], 1, material.getKSpec());
	glUniform3fv(unilocate[6], 1, material.getKAmb());
	glUniform1f(unilocate[7], material.getKshi());

	glUniform3fv(unilocate[8], 1, light.getPosition());
	glUniform3fv(unilocate[9], 1, light.getColor());
	glUniform3fv(unilocate[10], 1, light.getVector());
	glUniform2fv(unilocate[11], 1, light.getSize());
	glUniform1f(unilocate[12], light.getEnergy());
	glUniform1f(unilocate[13], light.getDistance());
	glUniform1f(unilocate[14], light.getSpot_size());
	glUniform1f(unilocate[15], light.getBlend());
	glUniform1f(unilocate[16], light.getGamma());
	glUniform1i(unilocate[17], light.getType());

	glUniform1i(unilocate[18], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.getAlbedoMap());

	glUniform1i(unilocate[19], 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.getNormalMap());

	glUniform1i(unilocate[20], 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material.getSpecularMap());

	glUniform1i(unilocate[21], 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, material.getCubeMap());

	glUniformMatrix4fv(unilocate[22], 1, GL_FALSE, material.getTex_Flag());

	glUniform1f(unilocate[23], object.getParameter()[1]);

	glUniform1i(unilocate[24], 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, material.getDepthMap());

	glUniform1f(unilocate[25], object.getParameter()[9]);
	glUniform1f(unilocate[26], object.getParameter()[10]);
	glUniform1f(unilocate[27], object.getParameter()[11]);
	glUniform1f(unilocate[28], object.getParameter()[12]);
	glUniform1f(unilocate[29], object.getParameter()[13]);
	glUniform1f(unilocate[30], object.getParameter()[14]);
	glUniform1f(unilocate[31], object.getParameter()[15]);
	glUniform1f(unilocate[32], object.getParameter()[2]);

	glUniform1f(unilocate[33], material.getRoughness());
	glUniform1f(unilocate[34], material.getTransparency());
	glUniform1f(unilocate[35], object.getParameter()[16]);
	glUniform1f(unilocate[36], object.getParameter()[17]);
}
void HairShader::draw(Basic3D& object, Light& light, Camera& camera)
{
	checkDrawObject<Basic3D>(object);
	object.setEnableFunc();
	object.setMatrix(camera);
	glUseProgram(this->getProgram());
	this->setUniform(object, light, *object.getMaterial());
	glBindVertexArray(object.getVao());
	for (int i = 0; i < int(object.getParameter()[0]); i++) {
		glDrawArrays(GL_LINE_STRIP_ADJACENCY, int(object.getVec_parameter().at(i)), int(object.getParameter()[1]));
	}
	glBindVertexArray(0);
	_glError(__FILE__, __LINE__);
}

void VolumeShader::createShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	loadShader(vert, frag, geom, tessc, tesse);

	//ÞŽ¿
	loadUnilocate(4, "kdiff", frag);
	loadUnilocate(5, "kspec", frag);
	loadUnilocate(6, "kamb", frag);
	loadUnilocate(7, "kshi", frag);

	//light î•ñ
	loadUnilocate(8, "lpos", vert);
	loadUnilocate(9, "lcolor", frag);
	loadUnilocate(10, "lvector", frag);
	loadUnilocate(11, "lsize", frag);
	loadUnilocate(12, "lenergy", frag);
	loadUnilocate(13, "ldistance", frag);
	loadUnilocate(14, "lspot_size", frag);
	loadUnilocate(15, "lblend", frag);
	loadUnilocate(16, "lgamma", frag);
	loadUnilocate(17, "ltype", frag);

	//
	loadUnilocate(18, "vmap", frag);
	loadUnilocate(19, "clip", frag);
	loadUnilocate(20, "texMatrix", frag);

	//parameter
	loadUnilocate(21, "threshold", frag);
	loadUnilocate(22, "slices", vert);


	loadUnilocate(23, "roughness", frag);
	loadUnilocate(24, "transparency", frag);

	loadUnilocate(25, "cubemap", frag);
	_glError(__FILE__, __LINE__);
}

void VolumeShader::setUniform(Volume3D &object, Light &light, Material &material)
{
	glUniformMatrix4fv(unilocate[0], 1, GL_FALSE, object.getMatrix()->model);
	glUniformMatrix4fv(unilocate[1], 1, GL_FALSE, object.getMatrix()->view);
	glUniformMatrix4fv(unilocate[2], 1, GL_FALSE, object.getMatrix()->proj);
	glUniformMatrix4fv(unilocate[3], 1, GL_FALSE, object.getMatrix()->adjoint);

	glUniform3fv(unilocate[4], 1, material.getKDiff());
	glUniform3fv(unilocate[5], 1, material.getKSpec());
	glUniform3fv(unilocate[6], 1, material.getKAmb());
	glUniform1f(unilocate[7], material.getKshi());

	glUniform3fv(unilocate[8], 1, light.getPosition());
	glUniform3fv(unilocate[9], 1, light.getColor());
	glUniform3fv(unilocate[10], 1, light.getVector());
	glUniform2fv(unilocate[11], 1, light.getSize());
	glUniform1f(unilocate[12], light.getEnergy());
	glUniform1f(unilocate[13], light.getDistance());
	glUniform1f(unilocate[14], light.getSpot_size());
	glUniform1f(unilocate[15], light.getBlend());
	glUniform1f(unilocate[16], light.getGamma());
	glUniform1i(unilocate[17], light.getType());

	glUniform1i(unilocate[18], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, object.getVdata());

	glUniformMatrix4fv(unilocate[19], 1, GL_FALSE, object.getMatrix()->clip);
	glUniformMatrix4fv(unilocate[20], 1, GL_FALSE, object.getMatrix()->texture);

	glUniform1f(unilocate[21], object.getParameter()[0]);
	glUniform1i(unilocate[22], int(object.getParameter()[1]));

	glUniform1f(unilocate[23], material.getRoughness());
	glUniform1f(unilocate[24], material.getTransparency());

	glUniform1i(unilocate[25], 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, material.getCubeMap());

	
	_glError(__FILE__, __LINE__);
}
void VolumeShader::draw(Volume3D& object, Light& light, Camera& camera)
{
	checkDrawObject<Volume3D>(object);
	object.setEnableFunc();
	object.setMatrix(camera);
	glUseProgram(this->getProgram());
	this->setUniform(object, light, *object.getMaterial());

	glBindVertexArray(object.getVao());
	glDrawElementsInstanced(GL_TRIANGLES, object.getVertices(), GL_UNSIGNED_INT, 0, int(object.getParameter()[1]));
	glBindVertexArray(0);
	_glError(__FILE__, __LINE__);
}

void SkyboxShader::createShader(const char* vert, const char* frag, const char* geom, const char* tessc, const char* tesse)
{
	loadShader(vert, frag, geom, tessc, tesse);
	loadUnilocate(3, "cubemap", frag);
	loadUnilocate(4, "adjoint", vert);
	_glError(__FILE__, __LINE__);
}

void SkyboxShader::setUniform(SkyBox &object)
{
	glUniformMatrix4fv(unilocate[1], 1, GL_FALSE, object.getView());
	glUniformMatrix4fv(unilocate[2], 1, GL_FALSE, object.getProj());
	glUniformMatrix4fv(unilocate[4], 1, GL_FALSE, object.getAdjo());

	glUniform1i(unilocate[3], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, object.getTex());
}
void SkyboxShader::draw(SkyBox& object, Camera& camera)
{
	object.setEnableFunc();
	object.setMatrix(camera);
	glUseProgram(this->getProgram());
	this->setUniform(object);

	glBindVertexArray(object.getVao());
	glDrawElements(GL_TRIANGLES, sizeof(GLuint[3]) * object.getIndices(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	_glError(__FILE__, __LINE__);
}