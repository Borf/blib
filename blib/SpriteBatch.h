#pragma once

#include <glm/glm.hpp>
#include <blib/Texture.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/math/Rectangle.h>
#include <blib/TextureMap.h>
#include <blib/RenderState.h>
#include <blib/json.hpp>
#include <queue>

namespace blib
{
	class Texture;
	class Shader;
	class Font;
	class Renderer;
	class RenderState;

	class ResourceManager;
	class VertexP2T2C4C4;
	class VBO;


	class SpriteBatch : public blib::gl::GlResizeRegister
	{
	private:

		typedef VertexP2T2C4C4 vertexDef;
	public:
		class Cache
		{
			blib::VBO* vbo = NULL;
		public:
			//pair<texture, end of what to draw>
			std::vector<std::pair<const Texture*, unsigned int> > materialIndices;
			std::vector<vertexDef> verts;
			~Cache();

			void initVbo(SpriteBatch& spriteBatch);
			void drawVbo(SpriteBatch& spriteBatch, const glm::mat4 &mat = glm::mat4());
		};
	private:


#define MAX_SPRITES 110000
		std::vector<vertexDef> vertices;
		std::vector<std::pair<const Texture*, unsigned int> > materialIndices;
		
		const Texture* currentTexture;
		Renderer* renderer;

		bool active;
		bool cacheActive;

		glm::mat4 matrix;
		FBO* fbo;

        size_t cacheStart;
		VBO* vbo;


	public:
		bool utf8 = true;
		int tabsize = 50;

		RenderState renderState;
		blib::Shader* shader;
		enum ShaderAttributes
		{
			ProjectionMatrix,
			Matrix,
			s_texture,
		};

		SpriteBatch(Renderer* renderer, ResourceManager* resourceManager, const RenderState &baseRenderState = RenderState());
		~SpriteBatch();

		virtual void begin(const glm::mat4 &matrix = glm::mat4(), FBO* fbo = NULL);
		virtual void end();

		virtual void startCache();
		virtual Cache* getCache(bool removeFromRenderQueue = false);
		virtual void drawCache(Cache* cache);


		virtual void draw(const Texture* sprite, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1, 1, 1, 1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual void draw(const Texture* sprite, const glm::mat4 &transform, const glm::vec2 &center,  const glm::vec4 &color = glm::vec4(1,1,1,1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual void draw(const Texture* sprite, const glm::mat4 &transform, const glm::vec2 &center, const blib::math::Rectangle &src, const glm::vec4 &color = glm::vec4(1,1,1,1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));

		virtual void draw(const TextureMap::TexInfo* sprite, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1,1,1,1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual void draw(const TextureMap::TexInfo* sprite, const glm::mat4 &transform, const glm::vec2 &center, const glm::vec4 &color = glm::vec4(1, 1, 1, 1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual void draw(const TextureMap::TexInfo* sprite, const glm::mat4 &transform, const glm::vec2 &center, const blib::math::Rectangle &src, const glm::vec4 &color = glm::vec4(1, 1, 1, 1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual glm::vec2 draw(const Font* font, const std::string& text, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1, 1, 1, 1));
		virtual glm::vec2 draw(const Font* font, const std::string& text, const glm::mat4 &transform, const glm::vec4 &color, glm::vec2 &cursor, int wrapWidth = -1, float maxWidth = 0);

		virtual void drawStretchyRect(Texture* sprite, const glm::mat4 &transform, const blib::math::Rectangle &src, const blib::math::Rectangle &innerSrc, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1,1,1,1));
		virtual void drawStretchyRect(Texture* sprite, const glm::mat4 &transform, json skin, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1,1,1,1));


		virtual void draw(const Texture* sprite, const glm::mat4 &transform, const std::vector<std::pair<glm::vec2, glm::vec2>> &coords, const glm::vec4 &color = glm::vec4(1, 1, 1, 1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual void draw(const Texture* sprite, const std::vector<std::pair<glm::vec2, glm::vec2>> &coords, const glm::vec4 &color = glm::vec4(1, 1, 1, 1), const glm::vec4 &colorOverlay = glm::vec4(1, 1, 1, 0));
		virtual void draw(const Texture* sprite, const glm::mat4 &transform, const std::vector<std::tuple<glm::vec2, glm::vec2, glm::vec4>> &coords);

		virtual void resizeGl( int width, int height, int offsetX, int offsetY );

		inline const glm::mat4& getMatrix() const { return matrix; };
	};
}
