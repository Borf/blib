#ifndef __VERTEX_H__
#define __VERTEX_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

template <int count>
class Vertex
{
protected:
public:
	static void setAttribPointers(int totalSize, void* base = NULL, int index = 0, int offset = 0)
	{
		glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, totalSize, (char*)base + offset);
	}
	static const int size() { return count; };
};


class VertexPosition3 : public Vertex<3>
{
public:
	glm::vec3 position;
	VertexPosition3(glm::vec3 position) : Vertex()
	{
		this->position = position;
	}
};

class VertexTexture2 : public Vertex<2>
{
public:
	glm::vec2 texCoord;
	VertexTexture2(glm::vec2 texCoord)
	{
		this->texCoord = texCoord;
	}
};


class VertexPosition3Texture2 : public VertexPosition3, public VertexTexture2
{
public:
	VertexPosition3Texture2(glm::vec3 position, glm::vec2 texCoord) : VertexPosition3(position), VertexTexture2(texCoord)
	{
	}

	static int setAttribPointers(int totalSize = size(), void* base = NULL, int index = 0, int offset = 0)
	{
		VertexPosition3::setAttribPointers(totalSize, base, index+0, offset);
		VertexTexture2::setAttribPointers(totalSize, base, index+1, offset);
		return index+2;
	}

	static const int size() { return VertexPosition3::size() + VertexTexture2::size(); };
};




/*
class VertexPosition : public Vertex
{
public:
	glm::vec3 position;

	VertexPosition(glm::vec3 position)
	{
		this->position = position;
	}

	static int getSize()
	{
		return sizeof(GL_FLOAT) * 3;
	}

	static int setVAO(int totalSize)
	{
		int index = 0;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*) 0);
		return index;
	}
};

class VertexPositionColor : public VertexPosition
{
public:
	glm::vec4 color;


	VertexPositionColor(glm::vec3 position, glm::vec4 color) : VertexPosition(position)
	{
		this->color = color;
	}

	static int getSize()
	{
		return VertexPosition::getSize() + sizeof(GL_FLOAT)*4;
	}
	static int setVAO(int totalSize)
	{
		int index = VertexPosition::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPosition::getSize());
		return index;
	}
};

class VertexPositionColorNormal : public VertexPositionColor
{
public:
	glm::vec3 normal;


	VertexPositionColorNormal(glm::vec3 position, glm::vec4 color, glm::vec3 normal) : VertexPositionColor(position, color)
	{
		this->normal = normal;
	}
	static int getSize()
	{
		return VertexPositionColor::getSize() + sizeof(GL_FLOAT)*3;
	}
	static int setVAO(int totalSize)
	{
		int index = VertexPositionColor::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionColor::getSize());
		return index;
	}
};

class VertexPositionNormal : public VertexPosition
{
public:
	glm::vec3 normal;


	VertexPositionNormal(glm::vec3 position, glm::vec3 normal) : VertexPosition(position)
	{
		this->normal = normal;
	}
	static int getSize()
	{
		return VertexPosition::getSize() + sizeof(GL_FLOAT)*3;
	}
	static int setVAO(int totalSize)
	{
		int index = VertexPosition::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPosition::getSize());
		return index;
	}
};


class VertexPositionColorTexture : public VertexPositionColor
{
public:
	glm::vec2 texCoord;

	VertexPositionColorTexture(glm::vec3 position, glm::vec4 color, glm::vec2 texCoord) : VertexPositionColor(position, color)
	{
		this->texCoord = texCoord;
	}
	static int getSize()
	{
		return VertexPositionColor::getSize() + sizeof(GL_FLOAT)*2;
	}
	static int setVAO(int totalSize)
	{
		int index = VertexPositionColor::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionColor::getSize());
		return index;
	}
};

class VertexPositionColorNormalTexture : public VertexPositionColorNormal
{
public:
	glm::vec2 texCoord;

	VertexPositionColorNormalTexture(glm::vec3 position, glm::vec4 color, glm::vec3 normal, glm::vec2 texCoord) : VertexPositionColorNormal(position, color, normal)
	{
		this->texCoord = texCoord;
	}
	
	static int getSize()
	{
		return VertexPositionColorNormal::getSize() + sizeof(GL_FLOAT)*2;
	}

	static int setVAO(int totalSize)
	{
		int index = VertexPositionColorNormal::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionColorNormal::getSize());
		return index;
	}

};


class VertexPositionTexture : public VertexPosition
{
public:
	glm::vec2 texCoord;
	VertexPositionTexture() : VertexPosition(glm::vec3(0,0,0))
	{
		texCoord = glm::vec2(0,0);
	}

	VertexPositionTexture(const VertexPositionTexture& other) : VertexPosition(other.position)
	{
		texCoord = other.texCoord;
	}


	VertexPositionTexture(glm::vec3 position, glm::vec2 texCoord) : VertexPosition(position)
	{
		this->texCoord = texCoord;
	}

	static int getSize()
	{
		return VertexPosition::getSize() + sizeof(GL_FLOAT)*2;
	}

	static int setVAO(int totalSize)
	{
		int index = VertexPosition::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPosition::getSize());
		return index;
	}

};

class VertexPositionColorNormalTextureTexture : public VertexPositionColorNormalTexture
{
public:
	glm::vec2 texCoord2;

	VertexPositionColorNormalTextureTexture(glm::vec3 position, glm::vec4 color, glm::vec3 normal, glm::vec2 texCoord, glm::vec2 texCoord2) : VertexPositionColorNormalTexture(position, color, normal, texCoord)
	{
		this->texCoord2 = texCoord2;
	}

	static int getSize()
	{
		return VertexPositionColorNormalTexture::getSize() + sizeof(GL_FLOAT)*2;
	}

	static int setVAO(int totalSize)
	{
		int index = VertexPositionColorNormalTexture::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionColorNormalTexture::getSize());
		return index;
	}

};
class VertexPositionNormalTexture : public VertexPositionNormal
{
public:
	glm::vec2 texCoord;

	VertexPositionNormalTexture(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord) : VertexPositionNormal(position, normal)
	{
		this->texCoord = texCoord;
	}

	static int getSize()
	{
		return VertexPositionNormal::getSize() + sizeof(GL_FLOAT)*2;
	}

	static int setVAO(int totalSize)
	{
		int index = VertexPositionNormal::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionNormal::getSize());
		return index;
	}
};

class VertexPositionNormalTextureTexture : public VertexPositionNormalTexture
{
public:
	glm::vec2 texCoord2;

	VertexPositionNormalTextureTexture(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec2 texCoord2) : VertexPositionNormalTexture(position, normal, texCoord)
	{
		this->texCoord2 = texCoord2;
	}

	static int getSize()
	{
		return VertexPositionNormalTexture::getSize() + sizeof(GL_FLOAT)*2;
	}

	static int setVAO(int totalSize)
	{
		int index = VertexPositionNormalTexture::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionNormalTexture::getSize());
		return index;
	}
};

class VertexPositionNormalTextureTextureColor : public VertexPositionNormalTextureTexture
{
public:
	glm::vec4 color;
	VertexPositionNormalTextureTextureColor(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec2 texCoord2, glm::vec4 color) : VertexPositionNormalTextureTexture(position, normal, texCoord, texCoord2)
	{
		this->color = color;
	}

	static int getSize()
	{
		return VertexPositionNormalTextureTexture::getSize() + sizeof(GL_FLOAT)*4;
	}

	static int setVAO(int totalSize)
	{
		int index = VertexPositionNormalTextureTexture::setVAO(totalSize)+1;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, totalSize, (void*) VertexPositionNormalTextureTexture::getSize());
		return index;
	}
};
*/
#endif
