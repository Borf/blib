#include "LineBatch.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>


namespace blib
{
	namespace gl
	{
		LineBatch::LineBatch()
		{
			active = false;
		}

		void LineBatch::initGl()
		{
			shader = Shader::fromData<Shader>("\
precision mediump float;\
attribute vec2 a_position;\
attribute vec4 a_color;\
varying vec4 color;\
uniform mat4 matrix;\
void main()\
{\
	color = a_color;\
	gl_Position = matrix * vec4(a_position,0.0,1.0);\
}\
", "\
precision mediump float;\
varying vec4 color;\
void main()\
{\
	gl_FragColor = color;\
}\
");
			vbo.setData(MAX_LINES*2, NULL, GL_STATIC_DRAW);
			unsigned short* indices = new unsigned short[MAX_LINES*2];
			for(int i = 0; i < MAX_LINES; i++)
				indices[i] = i;
			vio.setData(MAX_LINES*2, indices, GL_STATIC_DRAW);
			delete[] indices;
		}

		void LineBatch::begin(glm::mat4 matrix)
		{
			assert(!active);
			active = true;
			lineCount = 0;
			this->matrix = matrix;
			vbo.mapData(GL_WRITE_ONLY);		
		}

		void LineBatch::end()
		{
			assert(active);
			vbo.unmapData();
			if(lineCount == 0)
				return;

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			shader->use();
			vbo.bind();
			vio.bind();
			vertexDef::setAttribPointers();

			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			shader->use();
			int lastIndex = 0;
			glDrawElements(GL_LINES, lineCount, GL_UNSIGNED_SHORT, 0);

			active = false;
		}



		void LineBatch::draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color, glm::mat4 transform )
		{
			assert(active);

			vbo[lineCount].position = glm::vec2(matrix * transform * glm::vec4(v1,0,1));
			vbo[lineCount].color = color;
//			vbo[spriteCount].position.z = (float)depth*0.01f;
			lineCount++;

			vbo[lineCount].position = glm::vec2(matrix * transform * glm::vec4(v2,0,1));
			vbo[lineCount].color = color;
//			vbo[spriteCount].position.z = (float)depth*0.01f;
			lineCount++;

		}


		void LineBatch::Shader::resizeGl( int width, int height )
		{
			use();
			setUniform("matrix", glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
		}

		void LineBatch::Shader::init()
		{
			link();
			use();
			setUniform("a_position", 0);
			setUniform("a_color", 1);
		}

	}
}