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
		active = false;
		this->renderer = renderer;
		shader = resourceManager->getResource<Shader>();
		shader->initFromData("\
attribute vec2 a_position;\n\
attribute vec4 a_color;\n\
varying vec4 color;\n\
uniform mat4 projectionmatrix;\n\
uniform mat4 matrix;\n\
void main()\n\
{\n\
	color = a_color;\n\
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);\n\
}\n\
", "\
varying vec4 color;\n\
void main()\n\
{\n\
	gl_FragColor = color;\n\
}\n\
");
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_color", 1);

		shader->setUniformName(Uniforms::matrix, "matrix", Shader::Mat4);
		shader->setUniformName(Uniforms::projectionMatrix, "projectionmatrix", Shader::Mat4);
		shader->finishUniformSetup();

		renderState.activeShader = shader;
		vbo = resourceManager->getResource<blib::VBO>();
		vbo->setVertexFormat<vertexDef>();
		renderState.activeVbo = vbo;
		verts.reserve(10000);
	}


	void LineBatch::begin(glm::mat4 matrix)
	{
		assert(!active);
		active = true;
		lineCount = 0;
		this->matrix = matrix;
	}

	void LineBatch::end()
	{
		assert(active);
		active = false;
		if(lineCount == 0)
			return;

		renderer->setVbo(vbo, verts);
		renderState.activeShader->setUniform(Uniforms::matrix, matrix);
		renderer->drawLines<vertexDef>(lineCount, renderState);
	}



	void LineBatch::draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color, glm::mat4 transform )
	{
		assert(active);
		verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(v1, 0, 1)), color));
		verts.push_back(vertexDef(glm::vec2(transform * glm::vec4(v1, 0, 1)), color));
		lineCount+=2;
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
/*				glm::vec2 center = (it->p1 + it->p2) / 2.0f;
				glm::vec2 normal = glm::normalize(glm::vec2(it->p2.y - it->p1.y, -(it->p2.x - it->p1.x)));

				verts[lineCount].position = glm::vec2(transform * glm::vec4(center,0,1));
				verts[lineCount].color = glm::clamp(color * 1.25f, 0, 1);
				lineCount++;

				verts[lineCount].position = glm::vec2(transform * glm::vec4(center + 10.0f * normal,0,1));
				verts[lineCount].color = glm::clamp(color * 1.25f, 0, 1);
				lineCount++;*/


			}

		}

	}


	void LineBatch::resizeGl( int width, int height )
	{
		renderState.activeShader->setUniform(Uniforms::projectionMatrix, glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
	}
}
