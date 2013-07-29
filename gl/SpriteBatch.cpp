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
uniform mat4 projectionmatrix;\
void main()\
{\
	texCoord = a_texture;\
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);\
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
		}

		void SpriteBatch::begin(glm::mat4 matrix)
		{
			assert(!active);
			active = true;
			depth = 0;
			spriteCount = 0;
			currentTexture = NULL;
			this->matrix = matrix;
			materialIndices.clear();
			vbo.mapData(GL_WRITE_ONLY);		
		}

		void SpriteBatch::end()
		{
			assert(active);
			vbo.unmapData();

			if(spriteCount == 0)
				return;

			materialIndices.push_back(std::pair<Texture*, unsigned short>(currentTexture, (spriteCount/4) * 6));


			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			shader->use();
			shader->setUniform("matrix", matrix);
			vbo.bind();
			vio.bind();
			vertexDef::setAttribPointers();

			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			int lastIndex = 0;
			for(size_t i = 0; i < materialIndices.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, materialIndices[i].first->texid);
				glDrawElements(GL_TRIANGLES, materialIndices[i].second - lastIndex, GL_UNSIGNED_SHORT, (GLvoid*)(lastIndex * sizeof(unsigned short)));
				lastIndex = materialIndices[i].second;
			}

			active = false;
		}



		void SpriteBatch::draw( Texture* texture, glm::mat4 transform, glm::vec2 center, blib::math::Rectangle src)
		{
			assert(active);

			float fw = (float)src.width();
			float fh = (float)src.height();

			if(currentTexture != texture && currentTexture != NULL)
				materialIndices.push_back(std::pair<Texture*, unsigned short>(currentTexture, (spriteCount/4) * 6));
			currentTexture = texture;

			vbo[spriteCount].position = glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*0 - center.y,								0,1));
			vbo[spriteCount].texCoord = glm::vec2(src.topleft.x,src.bottomright.y);
//			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			vbo[spriteCount].position = glm::vec2(transform * glm::vec4(fw*0 - center.x,						fh*texture->originalHeight - center.y,		0,1));
			vbo[spriteCount].texCoord = glm::vec2(src.topleft.x,src.topleft.y);
//			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			vbo[spriteCount].position = glm::vec2(transform * glm::vec4(fw*texture->originalWidth - center.x,	fh*0 - center.y,							0,1));
			vbo[spriteCount].texCoord = glm::vec2(src.bottomright.x,src.bottomright.y);
//			vbo[spriteCount].position.z = (float)depth*0.01f;
			spriteCount++;

			vbo[spriteCount].position = glm::vec2(transform * glm::vec4(fw*texture->originalWidth - center.x,	fh*texture->originalHeight - center.y,	0,1));
			vbo[spriteCount].texCoord = glm::vec2(src.bottomright.x,src.topleft.y);
//			vbo[spriteCount].position.z = (float)depth*0.01f;
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
			setUniform("projectionmatrix", glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
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