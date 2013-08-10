#include "Image.h"

#include <gl/glew.h>

#include <blib/wm/wm.h>
#include <blib/gl/Texture.h>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			Image::Image(Texture* texture )
			{
				this->texture = texture;
				this->width = 100;
				this->height = 100;
			}



			void Image::draw( gl::SpriteBatch &spriteBatch)
			{
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