#pragma once

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

class Texture
{
protected:
	Texture();
	void fromData(unsigned char* data, int width, int height);
	void fromFile(std::string fileName, int loadOptions);

	Texture(std::string fileName, int loadOptions = 0);
	~Texture(void);
public:
	enum LoadOptions
	{
		NoResize = 1,
		KeepData = 2,
		LoadLater = 4,
		TextureWrap = 8,
	};

	static std::map<std::string, Texture*> textureCache;
	static Texture* loadCached(std::string fileName, int loadOptions = 0);
	static void clearCache();



	unsigned char* data;

	GLuint texid;
	int width;
	int height;

	int originalWidth;
	int originalHeight;
	

	Texture(unsigned char* data, int width, int height);
};


class TextureMap
{
public:
	int width;
	int height;

	class TexInfo
	{
	public:
		TexInfo(TextureMap* texMap);
		TextureMap* texMap;
		glm::vec2 t1;
		glm::vec2 t2;


		int x,y,width,height;
	};
	GLuint texid;
	std::map<std::string, TexInfo*> info;


	bool* taken;
	inline bool &isTaken(int x, int y);

	void save(std::string filename);

	TextureMap();
	~TextureMap();
	TexInfo* addTexture(std::string filename);
};


class TextureAtlas : public Texture
{
	struct ImageData
	{
		unsigned char* data;
		int width;
		int height;
		int depth;
	};
	std::vector<ImageData> imageData;

	int cx, cy;

public:
	TextureAtlas(std::vector<std::string> filenames );

	glm::vec2 topleft(int i);
	glm::vec2 size(int i);
	int frameCount();
};
