#include <blib/SpriteBatch.h>
#include <blib/Shader.h>
#include <blib/Font.h>
#include <blib/Renderer.h>
#include <blib/Texture.h>
#include <blib/ResourceManager.h>
#include <blib/gl/Vertex.h>
#include <blib/util/Log.h>
#include <blib/util/Thread.h>
#include <blib/Math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <blib/json.h>

using blib::util::Log;


namespace blib
{
	SpriteBatch::SpriteBatch(Renderer* renderer, ResourceManager* resourceManager, const RenderState &baseRenderState)
	{
		active = false;
		cacheActive = false;
		this->renderer = renderer;

		renderState = baseRenderState;
		renderState.depthTest = false;
		renderState.blendEnabled = true;
		renderState.srcBlendColor = blib::RenderState::SRC_ALPHA;
		renderState.srcBlendAlpha = blib::RenderState::SRC_ALPHA;
		renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;

		shader = resourceManager->getResource<Shader>("SpriteBatch");
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_texture", 1);
		shader->bindAttributeLocation("a_color", 2);
		shader->bindAttributeLocation("a_colorOverlay", 3);
		shader->setUniformName(ProjectionMatrix, "projectionmatrix", Shader::Mat4);
		shader->setUniformName(Matrix, "matrix", Shader::Mat4);
		shader->setUniformName(s_texture, "s_texture", Shader::Int);
		shader->finishUniformSetup();

		shader->setUniform(s_texture, 0);
		renderState.activeShader = shader;
		vbo = resourceManager->getResource<blib::VBO>();
		vbo->setVertexFormat<vertexDef>();
		renderState.activeVbo = vbo;
		vertices.reserve(MAX_SPRITES);
	}

	SpriteBatch::~SpriteBatch()
	{
		blib::ResourceManager::getInstance().dispose(vbo);
		blib::ResourceManager::getInstance().dispose(shader);
	}

	void SpriteBatch::begin(const glm::mat4 &matrix, FBO* fbo)
	{
		//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
		assert(!active);
		active = true;
		currentTexture = NULL;
		cacheActive = false;
		this->matrix = matrix;
		this->fbo = fbo;
		materialIndices.clear();
		vertices.clear();
	}

	void SpriteBatch::end()
	{
	//	assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
		assert(active);
		active = false;

		if(vertices.size() == 0)
			return;
		blib::FBO* oldFbo = renderState.activeFbo;
		if (fbo)
			renderState.activeFbo = fbo;

		renderer->setVbo(vbo, vertices);

		materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)vertices.size()));
		if (renderState.activeShader == shader)
			renderState.activeShader->setUniform(Matrix, matrix);
		int lastIndex = 0;
		for(size_t i = 0; i < materialIndices.size(); i++)
		{
			renderState.activeTexture[0] = const_cast<Texture*>(materialIndices[i].first);
			//renderer->drawTriangles<vertexDef>(&vertices[lastIndex], materialIndices[i].second - lastIndex, renderState);
			renderer->drawTriangles<vertexDef>(lastIndex, materialIndices[i].second - lastIndex, renderState);
			lastIndex = materialIndices[i].second;
		}
		renderState.activeFbo = oldFbo;
		active = false;
	}


	//TODO: make overload without src rectangle, so it doesn't have to clean it up
	void SpriteBatch::draw( const Texture* texture, const glm::mat4 &transform, const glm::vec2 &center, const blib::math::Rectangle &src, const glm::vec4 &color, const glm::vec4 &colorOverlay)
	{
//		assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
		assert(active);

		float fw = (float)src.width();
		float fh = (float)src.height();

		if(currentTexture != texture && currentTexture != NULL)
			materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)vertices.size()));
		currentTexture = texture;

		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*0 - center.y,								0,1)),	glm::vec2(src.topleft.x,src.topleft.y), color, colorOverlay)); // 1
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*texture->originalHeight - center.y,		0,1)),		glm::vec2(src.topleft.x,src.bottomright.y), color, colorOverlay)); // 2
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*texture->originalWidth - center.x,	fh*0 - center.y,							0,1)),		glm::vec2(src.bottomright.x,src.topleft.y), color, colorOverlay)); // 3

		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*texture->originalHeight - center.y,		0,1)), 		glm::vec2(src.topleft.x,src.bottomright.y), color, colorOverlay)); // 2
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*texture->originalWidth - center.x,	fh*0 - center.y,							0,1)),		glm::vec2(src.bottomright.x,src.topleft.y), color, colorOverlay)); // 3
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*texture->originalWidth - center.x,	fh*texture->originalHeight - center.y,	0,1)),			glm::vec2(src.bottomright.x,src.bottomright.y), color, colorOverlay)); //4
	}

	void SpriteBatch::draw( const Texture* texture, const glm::mat4 &transform, const glm::vec4 &color, const glm::vec4 &colorOverlay)
	{
		draw(texture, transform, glm::vec2(0,0), blib::math::Rectangle(0,0,1,1), color, colorOverlay);
	}
	void SpriteBatch::draw( const Texture* texture, const glm::mat4 &transform, const glm::vec2 &center, const glm::vec4 &color, const glm::vec4 &colorOverlay)
	{
		draw(texture, transform, center, blib::math::Rectangle(0,0,1,1), color, colorOverlay);
	}


	void SpriteBatch::draw( const TextureMap::TexInfo* texture, const glm::mat4 &transform, const glm::vec2 &center, const glm::vec4 &color, const glm::vec4 &colorOverlay)
	{
		//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
		assert(active);

		float fw = (float)1;
		float fh = (float)1;

		if(currentTexture != texture->texMap && currentTexture != NULL)
			materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)vertices.size()));
		currentTexture = texture->texMap;

		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*0 - center.y,					0,1)),		glm::vec2(texture->t1.x,texture->t1.y), color, colorOverlay)); // 1
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*texture->height - center.y,		0,1)),		glm::vec2(texture->t1.x,texture->t2.y), color, colorOverlay)); // 2
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*texture->width - center.x,			fh*0 - center.y,					0,1)),		glm::vec2(texture->t2.x,texture->t1.y), color, colorOverlay)); // 3

		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*texture->height - center.y,		0,1)), 		glm::vec2(texture->t1.x,texture->t2.y), color, colorOverlay)); // 2
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*texture->width - center.x,			fh*0 - center.y,					0,1)),		glm::vec2(texture->t2.x,texture->t1.y), color, colorOverlay)); // 3
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(fw*texture->width - center.x,			fh*texture->height - center.y,		0,1)),		glm::vec2(texture->t2.x,texture->t2.y), color, colorOverlay)); //4
	}

	void SpriteBatch::draw(const TextureMap::TexInfo* texture, const glm::mat4 &transform, const glm::vec2 &center, const blib::math::Rectangle& rect, const glm::vec4 &color, const glm::vec4 &colorOverlay)
	{
		//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
		assert(active);

		glm::vec2 spriteSize = rect.size() * glm::vec2(texture->width, texture->height);
		glm::vec2 texSize = (texture->t2 - texture->t1) * rect.size();

		glm::vec2 tl = texture->t1 + rect.topleft * (texture->t2 - texture->t1);

		if (currentTexture != texture->texMap && currentTexture != NULL)
			materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)vertices.size()));
		currentTexture = texture->texMap;

		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(spriteSize.x * 0 - center.x,	spriteSize.y * 0 - center.y, 0, 1)),	glm::vec2(tl.x + texSize.x * 0,	tl.y + texSize.y * 0), color, colorOverlay)); // 1
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(spriteSize.x * 0 - center.x,	spriteSize.y * 1 - center.y, 0, 1)),	glm::vec2(tl.x + texSize.x * 0,	tl.y + texSize.y * 1), color, colorOverlay)); // 2
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(spriteSize.x * 1 - center.x,	spriteSize.y * 0 - center.y, 0, 1)),	glm::vec2(tl.x + texSize.x * 1,	tl.y + texSize.y * 0), color, colorOverlay)); // 3

		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(spriteSize.x * 0 - center.x,	spriteSize.y * 1 - center.y, 0, 1)),	glm::vec2(tl.x + texSize.x * 0,	tl.y + texSize.y * 1), color, colorOverlay)); // 2
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(spriteSize.x * 1 - center.x,	spriteSize.y * 0 - center.y, 0, 1)),	glm::vec2(tl.x + texSize.x * 1,	tl.y + texSize.y * 0), color, colorOverlay)); // 3
		vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(spriteSize.x * 1 - center.x,	spriteSize.y * 1 - center.y, 0, 1)),	glm::vec2(tl.x + texSize.x * 1,	tl.y + texSize.y * 1), color, colorOverlay)); // 4
	}

	void SpriteBatch::draw( const TextureMap::TexInfo* texture, const glm::mat4 &transform, const glm::vec4 &color, const glm::vec4 &colorOverlay)
	{
		draw(texture, transform, glm::vec2(0,0), color, colorOverlay);
	}


	glm::vec2 SpriteBatch::draw(const Font* font, const std::string &text, const glm::mat4 &transform, const glm::vec4 &color)
	{
		glm::vec2 bla;
		return draw(font, text, transform, color, bla, -1);
	}


	glm::vec2 SpriteBatch::draw(const Font* font, const std::string &text, const glm::mat4 &transform, const glm::vec4 &color, glm::vec2 &cursor, int wrapWidth)
	{
		glm::vec2 texFactor(1.0f / font->texture->width, 1.0f / font->texture->height);

		float x = cursor.x;
		float y = cursor.y;
		int lineHeight = 12;
		for(size_t i = 0; i < text.size(); i++)
		{
			if (text[i] == '\n' || (x > wrapWidth && wrapWidth != -1))
			{
				x = 0;
				y += lineHeight;
				lineHeight = 12;
			}
			if(font->charmap.find(text[i]) == font->charmap.end())
				continue;
			const Glyph* g = font->getGlyph(text[i]);
			lineHeight = glm::max(lineHeight, g->height);
			draw(font->texture, glm::translate(transform, glm::vec3(x+g->xoffset,y+g->yoffset,0)), glm::vec2(0,0), blib::math::Rectangle(g->x*texFactor.x,g->y*texFactor.y,g->width*texFactor.x,g->height*texFactor.y), color);

			x+=g->xadvance;
		}

		return glm::vec2(x, y);
	}

	void SpriteBatch::draw(const Texture* texture, const glm::mat4 &transform, const std::vector<std::pair<glm::vec2, glm::vec2>> &coords, const glm::vec4 &color /*= glm::vec4(1,1,1,1)*/, const glm::vec4 &colorOverlay)
	{
		if (currentTexture != texture && currentTexture != NULL)
			materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)vertices.size()));
		currentTexture = texture;

		for (const std::pair<glm::vec2, glm::vec2> &coord : coords)
			vertices.push_back(vertexDef(glm::vec2(transform * glm::vec4(coord.first,0,1)), coord.second, color, colorOverlay));
	}

	void SpriteBatch::draw(const Texture* texture, const glm::mat4 &transform, const std::vector<std::tuple<glm::vec2, glm::vec2, glm::vec4>> &coords)
	{
		if (currentTexture != texture && currentTexture != NULL)
			materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)vertices.size()));
		currentTexture = texture;

		for (const std::tuple<glm::vec2, glm::vec2, glm::vec4> &coord : coords)
			vertices.push_back(vertexDef(std::get<0>(coord), std::get<1>(coord), std::get<2>(coord), glm::vec4(1,1,1,0)));
	}
	void SpriteBatch::drawStretchyRect( Texture* sprite, const glm::mat4 &transform, const blib::math::Rectangle &src, const blib::math::Rectangle &innerSrc, const glm::vec2 &size, const glm::vec4 &color )
	{

		glm::vec2 factor(1.0f / sprite->originalWidth, 1.0f / sprite->originalHeight);

		glm::vec2 marginBottomRight(src.bottomright - innerSrc.bottomright);
		glm::vec2 marginTopLeft(innerSrc.topleft - src.topleft);

		float facWidth = (size.x-marginBottomRight.x-marginTopLeft.x) / innerSrc.width();
		float facHeight = (size.y-marginBottomRight.y-marginTopLeft.y) / innerSrc.height();

		draw(sprite, transform, glm::vec2(0,0), blib::math::Rectangle(src.topleft * factor, innerSrc.topleft * factor), color); //topleft
		draw(sprite, glm::translate(transform, glm::vec3(size.x - marginBottomRight.x, 0,0)), glm::vec2(0,0), blib::math::Rectangle(glm::vec2(innerSrc.bottomright.x , src.topleft.y) * factor, glm::vec2(src.bottomright.x, innerSrc.topleft.y) * factor), color); //topright
		draw(sprite, glm::translate(transform, glm::vec3(0, size.y - marginBottomRight.y,0)), glm::vec2(0,0), blib::math::Rectangle(glm::vec2(src.topleft.x, innerSrc.bottomright.y) * factor, glm::vec2(innerSrc.topleft.x, src.bottomright.y) * factor), color); //bottomleft
		draw(sprite, glm::translate(transform, glm::vec3(size.x - marginBottomRight.x, size.y - marginBottomRight.y,0)), glm::vec2(0,0), blib::math::Rectangle(innerSrc.bottomright * factor, src.bottomright * factor), color); //bottomright

		draw(sprite, glm::scale(glm::translate(transform, glm::vec3(marginTopLeft.x, 0,0)), glm::vec3(facWidth,1,1)), glm::vec2(0,0), blib::math::Rectangle(glm::vec2(innerSrc.topleft.x , src.topleft.y) * factor, glm::vec2(innerSrc.bottomright.x,innerSrc.topleft.y) * factor), color); //top
		draw(sprite, glm::scale(glm::translate(transform, glm::vec3(marginTopLeft.x, size.y - marginBottomRight.y,0)), glm::vec3(facWidth,1,1)), glm::vec2(0,0), blib::math::Rectangle(glm::vec2(innerSrc.topleft.x , innerSrc.bottomright.y) * factor, glm::vec2(innerSrc.bottomright.x,src.bottomright.y) * factor), color); //bottom

		draw(sprite, glm::scale(glm::translate(transform, glm::vec3(0,marginTopLeft.y,0)), glm::vec3(1,facHeight,1)), glm::vec2(0,0), blib::math::Rectangle(glm::vec2(src.topleft.x , innerSrc.topleft.y) * factor, glm::vec2(innerSrc.topleft.x,innerSrc.bottomright.y) * factor), color); //left
		draw(sprite, glm::scale(glm::translate(transform, glm::vec3(size.x - marginBottomRight.x, marginTopLeft.y,0)), glm::vec3(1,facHeight,1)), glm::vec2(0,0), blib::math::Rectangle(glm::vec2(innerSrc.bottomright.x, innerSrc.topleft.y) * factor, glm::vec2(src.bottomright.x,innerSrc.bottomright.y) * factor), color); //right

		draw(sprite, glm::scale(glm::translate(transform, glm::vec3(marginTopLeft,0)), glm::vec3(facWidth,facHeight,1)), glm::vec2(0,0), blib::math::Rectangle(innerSrc.topleft * factor, innerSrc.bottomright * factor), color); //center
	}

	void SpriteBatch::drawStretchyRect(Texture* sprite, const glm::mat4 &transform, json::Value skin, const glm::vec2 &size, const glm::vec4 &color)
	{
		drawStretchyRect(
			sprite, 
			transform, 
			blib::math::Rectangle(glm::vec2(skin["left"]["pos"].asInt(), skin["top"]["pos"].asInt()), glm::vec2(skin["right"]["pos"].asInt() + skin["right"]["width"].asInt(), skin["bottom"]["pos"].asInt() + skin["bottom"]["height"].asInt())), 
			blib::math::Rectangle(glm::vec2(skin["left"]["pos"].asInt()+skin["left"]["width"].asInt(), skin["top"]["pos"].asInt()+skin["top"]["height"].asInt()), glm::vec2(skin["right"]["pos"].asInt(), skin["bottom"]["pos"].asInt())), 
			size, 
			color);
	}

	void SpriteBatch::startCache()
	{
		assert(active);
		assert(!cacheActive);
		cacheActive = true;
		cacheStart = vertices.size();
	}

	SpriteBatch::Cache* SpriteBatch::getCache()
	{
		assert(cacheActive);
		assert(active);
		cacheActive = false;

		Cache* cache = new Cache();
		if(cacheStart == vertices.size())
		{
			cache->materialIndices.clear();
			return cache;
		}

		cache->verts.insert(cache->verts.begin(), vertices.begin() + cacheStart, vertices.end());
		std::pair<const Texture*, unsigned short> p(currentTexture, (unsigned short)vertices.size());
		cache->materialIndices.push_back(p);

		for(size_t i = 0; i < cache->materialIndices.size(); i++)
			cache->materialIndices[i].second -= (unsigned short)cacheStart;
		while(!cache->materialIndices.empty() && cache->materialIndices[0].second < 0)
			cache->materialIndices.erase(cache->materialIndices.begin());
			

		return cache;
	}


	void SpriteBatch::drawCache(Cache* cache)
	{
		assert(active);

		if(cache->materialIndices.size() == 0)
			return;

		size_t currentSize = vertices.size();
		vertices.insert(vertices.end(), cache->verts.begin(), cache->verts.end());


		if(currentTexture != cache->materialIndices[0].first && currentTexture != NULL)
			materialIndices.push_back(std::pair<const Texture*, unsigned short>(currentTexture, (unsigned short)currentSize));
		currentTexture = cache->materialIndices[0].first;



		for(size_t i = 0; i < cache->materialIndices.size()-1; i++)
		{
			if(cache->materialIndices[i].second < 0)
				throw "argh";
			materialIndices.push_back(cache->materialIndices[i]);
			materialIndices[materialIndices.size()-1].second += (unsigned short)currentSize;
		}
		currentTexture = cache->materialIndices[cache->materialIndices.size()-1].first;

	}


	void SpriteBatch::resizeGl( int width, int height )
	{
		if (renderState.activeShader == shader)
			renderState.activeShader->setUniform(ProjectionMatrix, glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
	}

	SpriteBatch::Cache::~Cache()
	{
		verts.clear();
		materialIndices.clear();
	}




	void SpriteBatch::Cache::initVbo(SpriteBatch& spriteBatch)
	{
		if (vbo == NULL)
		{
			vbo = blib::ResourceManager::getInstance().getResource<blib::VBO>();
			vbo->setVertexFormat<vertexDef>();
		}
		spriteBatch.renderer->setVbo(vbo, verts);
	}

	void SpriteBatch::Cache::drawVbo(SpriteBatch& spriteBatch)
	{
		blib::RenderState renderState = spriteBatch.renderState;
		renderState.activeVbo = vbo;
		int lastIndex = 0;
		if (renderState.activeShader == spriteBatch.shader)
			renderState.activeShader->setUniform(Matrix, spriteBatch.matrix);
		for (size_t i = 0; i < materialIndices.size(); i++)
		{
			renderState.activeTexture[0] = const_cast<Texture*>(materialIndices[i].first);
			spriteBatch.renderer->drawTriangles<vertexDef>(lastIndex, materialIndices[i].second - lastIndex, renderState);
			lastIndex = materialIndices[i].second;
		}
	}

}
