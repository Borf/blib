#include "SpriteBatch.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>


namespace blib
{
	namespace gl
	{
		SpriteBatch::SpriteBatch()
		{
			active = false;
		}

		void SpriteBatch::initGl()
		{
			shader = Shader::fromData<Shader>("\
precision mediump float;\
attribute vec2 a_position;\
attribute vec2 a_texture;\
varying vec2 texCoord;\
uniform mat4 matrix;\
void main()\
{\
	texCoord = a_texture;\
	gl_Position = matrix * vec4(a_position,0.0,1.0);\
}\
", "\
precision mediump float;\
uniform sampler2D s_texture;\
varying vec2 texCoord;\
void main()\
{\
	gl_FragColor = texture2D(s_texture, texCoord);\
}\
");
			vbo.setData(MAX_SPRITES*4, NULL, GL_STATIC_DRAW);
			int index = 0;
			unsigned short* indices = new unsigned short[MAX_SPRITES*6];
			for(int i = 0; i < MAX_SPRITES; i+=6)
			{
				indices[i+0] = index+0;
				indices[i+1] = index+1;
				indices[i+2] = index+2;

				indices[i+3] = index+1;
				indices[i+4] = index+3;
				indices[i+5] = index+2;

				index+=4;
			}
			vio.setData(MAX_SPRITES*6, indices, GL_STATIC_DRAW);
			delete[] indices;

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			shader->use();
			vbo.bind();
			vio.bind();
			vertexDef::setAttribPointers();
		}

		void SpriteBatch::begin()
		{
			assert(!active);
			active = true;
			depth = 0;
			spriteCount = 0;
			vbo.mapData(GL_WRITE_ONLY);		
		}

		void SpriteBatch::end()
		{
			assert(active);
			vbo.unmapData();
			glDrawElements(GL_TRIANGLES, (spriteCount/4)*6, GL_UNSIGNED_SHORT, 0);
			active = false;
		}



		void SpriteBatch::draw( Texture* texture, glm::mat4 transform )
		{
			assert(active);
			vbo[spriteCount].position = glm::vec3(transform * glm::vec4(0,0,0,1));
			vbo[spriteCount].texCoord = glm::vec2(0,0);
			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			vbo[spriteCount].position = glm::vec3(transform * glm::vec4(0,texture->originalHeight,0,1));
			vbo[spriteCount].texCoord = glm::vec2(0,1);
			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			vbo[spriteCount].position = glm::vec3(transform * glm::vec4(texture->originalWidth,0,0,1));
			vbo[spriteCount].texCoord = glm::vec2(1,0);
			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			vbo[spriteCount].position = glm::vec3(transform * glm::vec4(texture->originalWidth,texture->originalHeight,0,1));
			vbo[spriteCount].texCoord = glm::vec2(1,1);
			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			depth++;
		}

		void SpriteBatch::draw( TextureMap::TexInfo* texture, glm::mat4 transform )
		{
			assert(active);

		}



		void SpriteBatch::Shader::resizeGl( int width, int height )
		{
			use();
			setUniform("matrix", glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
		}

		void SpriteBatch::Shader::init()
		{
			link();
			use();
			setUniform("a_position", 0);
			setUniform("a_texture", 1);
		}

	}
}