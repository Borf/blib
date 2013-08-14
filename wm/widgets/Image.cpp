#include "Image.h"

#include <gl/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <blib/wm/wm.h>
#include <blib/gl/Texture.h>
#include <blib/gl/SpriteBatch.h>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			Image::Image(gl::Texture* texture )
			{
				this->texture = texture;
				this->width = 100;
				this->height = 100;
			}



			void Image::draw( gl::SpriteBatch &spriteBatch, glm::mat4 matrix)
			{
				spriteBatch.draw(texture, glm::translate(matrix, glm::vec3(x,y,0)));

			/*	glBindTexture(GL_TEXTURE_2D, texture->texid);
				shader->useTexture(true);

				glBegin(GL_QUADS);
				glVertexAttrib2f(2, 0.0f, 0.0f);			glVertexAttrib3f(0, (float)(x),			(float)(y), 0);
				glVertexAttrib2f(2, 1.0f, 0.0f);			glVertexAttrib3f(0, (float)(x+width),	(float)(y), 0);
				glVertexAttrib2f(2, 1.0f, 1.0f);			glVertexAttrib3f(0, (float)(x+width),	(float)(y+height), 0);
				glVertexAttrib2f(2, 0.0f, 1.0f);			glVertexAttrib3f(0, (float)(x),			(float)(y+height), 0);
				glEnd();

				shader->setColor(glm::vec4(0,0,0,1));
				glBegin(GL_LINE_LOOP);
					glVertexAttrib3f(0, (float)(x),			(float)(y), 0);
					glVertexAttrib3f(0, (float)(x+width),	(float)(y), 0);
					glVertexAttrib3f(0, (float)(x+width),	(float)(y+height), 0);
					glVertexAttrib3f(0, (float)(x),			(float)(y+height), 0);
				glEnd();
				shader->setColor(glm::vec4(1,1,1,1));*/

			}

		}
	}
}