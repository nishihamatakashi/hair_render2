#include "texture.h"

void LoadTexture(GLuint &tex, GLsizei width, GLsizei height, GLenum internal, GLenum format, const GLvoid *image)
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// �A���t�@�`�����l�������Ă���� 4 �o�C�g���E�ɐݒ肷��
	//4�o�C�g���X���C�h���ēǂݍ���ł���
	glPixelStorei(GL_UNPACK_ALIGNMENT, (format == GL_BGRA || format == GL_RGBA) ? 4 : 1);

	// �e�N�X�`�������蓖�Ă�
	glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	// �o�C���j�A�i�~�b�v�}�b�v�Ȃ��j�C�G�b�W�ŃN�����v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadCubeTexture(GLuint& tex, GLsizei *width, GLsizei *height, GLenum *internal, GLenum *format, const GLvoid *px,
	const GLvoid *nx, const GLvoid *py, const GLvoid *ny, const GLvoid *pz, const GLvoid *nz)
{

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	if (px) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT, 0, internal[0], width[0], height[0], 0, format[0], GL_UNSIGNED_BYTE, px);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT, 0, internal[1], width[1], height[1], 0, format[1], GL_UNSIGNED_BYTE, py);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT, 0, internal[2], width[2], height[2], 0, format[2], GL_UNSIGNED_BYTE, pz);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT, 0, internal[3], width[3], height[3], 0, format[3], GL_UNSIGNED_BYTE, nx);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT, 0, internal[4], width[4], height[4], 0, format[4], GL_UNSIGNED_BYTE, ny);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT, 0, internal[5], width[5], height[5], 0, format[5], GL_UNSIGNED_BYTE, nz);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLubyte *LoadTga(const char *name, GLsizei *width, GLsizei *height, GLenum *format)
{
	if (name == 0) return NULL;
	// �t�@�C�����J��
	//ifstream:�t�@�C���ǂݍ��݂̌^(�N���X)�C�o�C�i���t�@�C���œǂݍ��ނƂ����Ӗ�
	std::ifstream file(name, std::ios::binary);

	// �t�@�C�����J���Ȃ�������߂�
	if (!file)
	{
		std::cerr << "Error: Can't open file: " << name << std::endl;
		return NULL;
	}

	// �w�b�_��ǂݍ���
	unsigned char header[18];
	file.read(reinterpret_cast<char *>(header), sizeof header);

	// �w�b�_�̓ǂݍ��݂Ɏ��s������߂�
	if (file.bad())
	{
		std::cerr << "Error: Can't read file header: " << name << std::endl;
		file.close();
		return NULL;
	}

	// ���ƍ���
	*width = header[13] << 8 | header[12];
	*height = header[15] << 8 | header[14];

	// �[�x
	const size_t depth(header[16] / 8);
	switch (depth)
	{
	case 1:
		*format = GL_RED;
		break;
	case 2:
		*format = GL_RG;
		break;
	case 3:
		*format = GL_BGR;
		break;
	case 4:
		*format = GL_BGRA;
		break;
	default:
		// ��舵���Ȃ��t�H�[�}�b�g��������߂�
		std::cerr << "Error: Unusable format: " << depth << std::endl;
		file.close();
		return NULL;
	}
	// �f�[�^�T�C�Y
	const size_t size(*width * *height * depth);
	if (size < 2) return NULL;

	// �ǂݍ��݂Ɏg�����������m�ۂ���
	GLubyte *const buffer(new(std::nothrow) GLubyte[size]);

	// ���������m�ۂł��Ȃ���Ζ߂�
	if (buffer == NULL)
	{
		std::cerr << "Error: Too large file: " << name << std::endl;
		file.close();
		return NULL;
	}

	// �f�[�^��ǂݍ���
	if (header[2] & 8)
	{
		// RLE
		size_t p(0);
		char c;
		while (file.get(c))
		{
			if (c & 0x80)
			{
				// run-length packet
				const size_t count((c & 0x7f) + 1);
				if (p + count * depth > size) break;
				char temp[4];
				file.read(temp, depth);
				for (size_t i = 0; i < count; ++i)
				{
					for (size_t j = 0; j < depth;) {
						buffer[p++] = temp[j++];
					}
				}
			}
			else
			{
				// raw packet
				const size_t count((c + 1) * depth);
				if (p + count > size) break;
				file.read(reinterpret_cast<char *>(buffer + p), count);
				p += count;
			}
		}
	}
	else
	{
		// �񈳏k
		file.read(reinterpret_cast<char *>(buffer), size);
	}

	// �ǂݍ��݂Ɏ��s���Ă�����x�����o��
	if (file.bad())
	{
		std::cerr << "Waring: Can't read image data: " << name << std::endl;
	}

	// �t�@�C�������
	file.close();

	// �摜��ǂݍ��񂾃�������Ԃ�
	return buffer;
}


void LoadImage(GLuint &tex, const char *name)
{
	// �摜�T�C�Y
	GLsizei width, height;

	// �摜�t�H�[�}�b�g
	GLenum format;

	GLenum internal = 0;

	// �摜��ǂݍ���
	const GLubyte *const image(LoadTga(name, &width, &height, &format));

	// �摜���ǂݍ��߂Ȃ�������߂�
	if (image == NULL) return;

	// internal == 0 �Ȃ�����t�H�[�}�b�g��ǂݍ��񂾃t�@�C���ɍ��킹��
	if (internal == 0)
	{
		switch (format)
		{
		case GL_BGR:
			internal = GL_RGB;
			break;
		case GL_BGRA:
			internal = GL_RGBA;
			break;
		default:
			internal = format;
			break;
		}
	}

	// �e�N�X�`���������ɓǂݍ���
	LoadTexture(tex, width, height, internal, format, image);

	// �摜�̓ǂݍ��݂Ɏg�������������������
	delete[] image;
}

void LoadTestMap(GLuint &tex, const char *name)
{
	// �摜�T�C�Y
	GLsizei width = 512;
	GLsizei height = 512;

	// �摜��ǂݍ���
	std::vector<float> image;
	image.resize(width * height * 4);
	for (size_t i = 0; i < image.size(); i++)
	{
		image.at(i) = 0.0f;
		if (i % 4 == 0)
		{
			image.at(i) = float(rand()) / RAND_MAX;
		}
		else if (i % 4 == 3)
		{
			image.at(i) = 1.0f;
		}
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// �e�N�X�`�������蓖�Ă�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, &image.at(0));

	// �o�C���j�A�i�~�b�v�}�b�v�Ȃ��j�C�G�b�W�ŃN�����v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadHeight(GLuint &tex, const char *name, float nz)
{
	// �摜�T�C�Y
	GLsizei width, height;

	// �摜�t�H�[�}�b�g
	GLenum format;

	// �����}�b�v�̉摜��ǂݍ���
	const GLubyte *const hmap(LoadTga(name, &width, &height, &format));

	GLenum internal = GL_RGBA;

	// �摜���ǂݍ��߂Ȃ�������߂�
	if (hmap == NULL) return;

	// ��f�̃o�C�g��
	int bytes;
	switch (format)
	{
	case GL_RED:
		bytes = 1;
		break;
	case GL_RG:
		bytes = 2;
		break;
	case GL_BGR:
		bytes = 3;
		break;
	case GL_BGRA:
		bytes = 4;
		break;
	default:
		bytes = 1;
		break;
	}

	// �������T�C�Y
	const GLsizei maxsize(width * height);

	// �@���}�b�v�̃��������m�ۂ���
	GLfloat(*const nmap)[4](new(std::nothrow) GLfloat[maxsize][4]);

	// ���������m�ۂł��Ȃ���Ζ߂�
	if (nmap == NULL)
	{
		delete[] hmap;
		return;
	}

	// �@���}�b�v�̍쐬
	for (GLsizei i = 0; i < maxsize; ++i)
	{
		const int x(i % width);
		const int y(i - x);
		const int o(i * bytes);
		const int u((y + (x + 1) % width) * bytes);
		const int v(((y + width) % maxsize + x) * bytes);

		// �אڂ����f�Ƃ̒l�̍���@���̐����ɗp����
		const float n[3] = { static_cast<float>(hmap[u] - hmap[o]), static_cast<float>(hmap[v] - hmap[o]), nz };

		// �@���̒��������߂Ă���
		const float nl(distance(n[0], n[1], n[2]));

		// �@�������߂�
		nmap[i][0] = n[0] / nl;
		nmap[i][1] = n[1] / nl;
		nmap[i][2] = n[2] / nl;
		nmap[i][3] = hmap[o];
	}

	// �����}�b�v�̉摜�̓ǂݍ��݂Ɏg�������������������
	delete[] hmap;

	// �����t�H�[�}�b�g�����������_�e�N�X�`���łȂ���� [0,1] �ɕϊ�����
	if (
		internal != GL_RGB16F &&
		internal != GL_RGBA16F &&
		internal != GL_RGB32F &&
		internal != GL_RGBA32F
		)
	{
		for (GLsizei i = 0; i < maxsize; ++i)
		{
			nmap[i][0] = nmap[i][0] * 0.5f + 0.5f;
			nmap[i][1] = nmap[i][1] * 0.5f + 0.5f;
			nmap[i][2] = nmap[i][2] * 0.5f + 0.5f;
			nmap[i][3] *= 0.0039215686f; // == 1/255
		}
	}

	// �e�N�X�`���I�u�W�F�N�g
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// nmap �� GLfloat �Ȃ̂� 4 �o�C�g���E�ɐݒ肷��
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// �e�N�X�`�������蓖�Ă�
	glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, GL_RGBA, GL_FLOAT, nmap);

	// �o�C���j�A�i�~�b�v�}�b�v�Ȃ��j�C�G�b�W�ŃN�����v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
	// �@���}�b�v�̍쐬�Ɏg�������������������
	delete[] nmap;
}

void LoadCubeImage(GLuint &tex, const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz)
{
	// �摜�T�C�Y
	GLsizei width[6], height[6];

	// �摜�t�H�[�}�b�g
	GLenum format[6];

	GLenum internal[6] = { 0,0,0,0,0,0 };

	// �摜��ǂݍ���
	const GLubyte *const image_px(LoadTga(px, &width[0], &height[0], &format[0]));
	const GLubyte *const image_nx(LoadTga(nx, &width[1], &height[1], &format[1]));
	const GLubyte *const image_py(LoadTga(py, &width[2], &height[2], &format[2]));
	const GLubyte *const image_ny(LoadTga(ny, &width[3], &height[3], &format[3]));
	const GLubyte *const image_pz(LoadTga(pz, &width[4], &height[4], &format[4]));
	const GLubyte *const image_nz(LoadTga(nz, &width[5], &height[5], &format[5]));

	// internal == 0 �Ȃ�����t�H�[�}�b�g��ǂݍ��񂾃t�@�C���ɍ��킹��
	for (int i = 0; i < 6; i++) {
		if (internal[i] == 0)
		{
			switch (format[i])
			{
			case GL_BGR:
				internal[i] = GL_RGB;
				break;
			case GL_BGRA:
				internal[i] = GL_RGBA;
				break;
			default:
				internal[i] = format[i];
				break;
			}
		}
	}

	// �e�N�X�`���������ɓǂݍ���
	LoadCubeTexture(tex, width, height, internal, format, image_px, image_nx, image_py, image_ny, image_pz, image_nz);

	// �摜�̓ǂݍ��݂Ɏg�������������������
	delete[] image_px;
	delete[] image_nx;
	delete[] image_py;
	delete[] image_ny;
	delete[] image_pz;
	delete[] image_nz;
}