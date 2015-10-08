#include "LineBatch.h"
#include "Shader.h"
#include <blib/Renderer.h>
#include <blib/IDrawableLine.h>
#include <blib/ResourceManager.h>

#include <glm/gtc/matrix_transform.hpp>


namespace blib
{
	LineBatch::LineBatch(Renderer* renderer, ResourceManager* resourceManager, const RenderState &baseRenderState)
	{
		renderState = baseRenderState;
		renderState.depthTest = false;
		renderState.blendEnabled = true;
		renderState.srcBlendColor = blib::RenderState::SRC_ALPHA;
		renderState.srcBlendAlpha = blib::RenderState::SRC_ALPHA;
		renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;

		active = false;
		this->renderer = renderer;
		shader = resourceManager->getResource<Shader>("LineBatch");
		
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_color", 1);

		shader->setUniformName(Uniforms::matrix, "matrix", Shader::Mat4);
		shader->setUniformName(Uniforms::projectionMatrix, "projectionmatrix", Shader::Mat4);
		shader->finishUniformSetup();

		renderState.activeShader = shader;
		vbo = resourceManager->getResource<blib::VBO>();
		vbo->setVertexFormat<vertexDef>();
		renderState.activeVbo = vbo;
		verts.reserve(1000000);
	}

	LineBatch::~LineBatch()
	{
		blib::ResourceManager::getInstance().dispose(vbo);
		blib::ResourceManager::getInstance().dispose(shader);
	}

	void LineBatch::begin(const glm::mat4 &matrix, float thickness)
	{
		assert(!active);
		active = true;
		this->thickness = thickness;
		this->matrix = matrix;
		verts.clear();
	}

	void LineBatch::end()
	{
		assert(active);
		active = false;
		if (verts.empty())
			return;
		
		renderState.activeTexture[0] = NULL;

		renderer->setVbo(vbo, verts);
		renderState.activeShader->setUniform(Uniforms::matrix, matrix);
		renderer->drawLines<vertexDef>(verts.size(), thickness, renderState);
	}



	void LineBatch::draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color, glm::mat4 transform )
	{
		assert(active);
		verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(v1, 0, 1)), color));
		verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(v2, 0, 1)), color));
	}

	void LineBatch::draw(const blib::IDrawableLine& drawable, glm::vec4 color, bool showNormal, glm::mat4 transform)
	{
		assert(active);
		const std::list<blib::IDrawableLine::LinePart>& lines = ((blib::IDrawableLine&)drawable).getLines(); //ewww
		for(std::list<blib::IDrawableLine::LinePart>::const_iterator it = lines.begin(); it != lines.end(); it++)
		{
			verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(it->p1, 0, 1)), color));
			verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(it->p2, 0, 1)), color));

			if(showNormal)
			{
				glm::vec2 center = (it->p1 + it->p2) / 2.0f;
				glm::vec2 normal = glm::normalize(glm::vec2(it->p2.y - it->p1.y, -(it->p2.x - it->p1.x)));

				verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(center,0,1)), glm::clamp(color * 1.25f, 0, 1)));
				verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(center + 10.0f * normal,0,1)), glm::clamp(color * 1.25f, 0, 1)));
			}

		}

	}


	void LineBatch::resizeGl( int width, int height )
	{
		renderState.activeShader->setUniform(Uniforms::projectionMatrix, glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
	}


	void LineBatch::startCache()
	{
		assert(active);
		assert(!cacheActive);
		cacheActive = true;
		cacheStart = verts.size();
	}

	LineBatch::Cache* LineBatch::getCache()
	{
		assert(cacheActive);
		assert(active);
		cacheActive = false;

		Cache* cache = new Cache();
		if (cacheStart == verts.size())
		{
			return cache;
		}

		cache->verts.insert(cache->verts.begin(), verts.begin() + cacheStart, verts.end());
		return cache;
	}


	void LineBatch::drawCache(Cache* cache)
	{
		assert(active);
		if (cache->verts.empty())
			return;
		size_t currentSize = verts.size();
		verts.insert(verts.end(), cache->verts.begin(), cache->verts.end());
	}



}
