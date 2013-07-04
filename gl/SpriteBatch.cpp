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
			int index = 0;
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
		}

		void SpriteBatch::initGl()
		{
			shader = Shader::fromData<Shader>("\
precision mediump float;\
attribute vec3 a_position;\
attribute vec2 a_texture;\
varying vec2 texCoord;\
uniform mat4 matrix;\
void main()\
{\
	texCoord = a_texture;\
	gl_Position = matrix * vec4(a_position,1.0);\
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


			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			shader->use();
			VertexPosition3Texture2::setAttribPointers(verts);


		}

		void SpriteBatch::begin()
		{
			assert(!active);
			active = true;
			depth = 0;
			spriteCount = 0;
		}

		void SpriteBatch::end()
		{
			assert(active);


			glDrawElements(GL_TRIANGLES, (spriteCount/4)*6, GL_UNSIGNED_SHORT, indices);



			active = false;
		}

		void SpriteBatch::draw( Texture* texture, glm::mat4 transform )
		{
			assert(active);

			verts[spriteCount].position = glm::vec3(transform * glm::vec4(0,0,0,1));
			verts[spriteCount].texCoord = glm::vec2(0,0);
			verts[spriteCount++].position.z = (float)depth*0.01f;
			verts[spriteCount].position = glm::vec3(transform * glm::vec4(0,texture->originalHeight,0,1));
			verts[spriteCount].texCoord = glm::vec2(0,1);
			verts[spriteCount++].position.z = (float)depth*0.01f;
			verts[spriteCount].position = glm::vec3(transform * glm::vec4(texture->originalWidth,0,0,1));
			verts[spriteCount].texCoord = glm::vec2(1,0);
			verts[spriteCount++].position.z = (float)depth*0.01f;
			verts[spriteCount].position = glm::vec3(transform * glm::vec4(texture->originalWidth,texture->originalHeight,0,1));
			verts[spriteCount].texCoord = glm::vec2(1,1);
			verts[spriteCount++].position.z = (float)depth*0.01f;
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
			setUniform("a_position", 0);
			setUniform("a_texture", 1);
			link();
		}

	}
}