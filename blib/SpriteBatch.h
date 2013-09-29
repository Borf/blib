#pragma once

#include <glm/glm.hpp>
#include <blib/Texture.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Shader.h>
#include <blib/math/Rectangle.h>
#include <blib/TextureMap.h>
#include <blib/RenderState.h>
#include <queue>

namespace Json { class Value; }

namespace blib
{
	class Texture;
	class Shader;
	class Font;
	class Renderer;
	class RenderState;

	class ResourceManager;
	class VertexP2T2C4;
	class VBO;


	class SpriteBatch : public blib::gl::GlResizeRegister
	{
	private:

		typedef VertexP2T2C4 vertexDef;
	public:
		class Cache
		{
		public:
			std::vector<std::pair<const Texture*, unsigned short> > materialIndices;
			std::vector<vertexDef> verts;
		};
	private:


#define MAX_SPRITES 110000
		std::vector<vertexDef> vertices;
		std::vector<std::pair<const Texture*, unsigned short> > materialIndices;
		
		const Texture* currentTexture;
		Renderer* renderer;

		bool active;
		bool cacheActive;

		glm::mat4 matrix;

		int cacheStart;


	public:
		RenderState renderState;
		blib::Shader* shader;

		SpriteBatch(Renderer* renderer, ResourceManager* resourceManager, const RenderState &baseRenderState = RenderState());

		virtual void begin(glm::mat4 matrix = glm::mat4());
		virtual void end();

		virtual void startCache();
		virtual Cache* getCache();
		virtual void drawCache(Cache* cache);

		virtual void draw(const Texture* sprite, const glm::mat4 &transform, const glm::vec2 &center = glm::vec2(0,0), const blib::math::Rectangle &src = blib::math::Rectangle(0,0,1,1), const glm::vec4 &color = glm::vec4(1,1,1,1));
		virtual void draw(const TextureMap::TexInfo* sprite, const glm::mat4 &transform, const glm::vec2 &center = glm::vec2(0,0), const glm::vec4 &color = glm::vec4(1,1,1,1));
		virtual void draw(const Font* font, const std::string& text, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1,1,1,1));

		virtual void drawStretchyRect(Texture* sprite, glm::mat4 transform, blib::math::Rectangle src, blib::math::Rectangle innerSrc, glm::vec2 size, glm::vec4 color = glm::vec4(1,1,1,1));
		virtual void drawStretchyRect(Texture* sprite, glm::mat4 transform, Json::Value skin, glm::vec2 size, glm::vec4 color = glm::vec4(1,1,1,1));

		virtual void resizeGl( int width, int height );
	};
}
