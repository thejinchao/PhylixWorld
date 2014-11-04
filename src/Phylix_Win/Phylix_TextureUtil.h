#pragma once

namespace Phylix
{

// opengl texture info
struct TextureInfo
{
	int		width;
	int		height;
	bool	alpha;	//�Ƿ����alphaͨ��
	GLuint	textureID;
};

// create texture from png file
bool createTextureFromPngFile(const char* fileName, TextureInfo& textureInfo);

}
