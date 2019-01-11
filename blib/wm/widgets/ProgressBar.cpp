#include "ProgressBar.h"

//#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <blib/wm/WM.h>
#include <blib/SpriteBatch.h>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			ProgressBar::ProgressBar( )
			{
				this->width = 100;
				this->height = 9;
				this->min = 0;
				this->max = 100;
				this->value = 50;
			}



			void ProgressBar::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				float fac = (this->value - this->min) / this->max;
				spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x, y, 0)), WM::getInstance()->skin["progress"]["back"], glm::vec2(width, height), glm::vec4(0.7f, 0.7f, 0.9f, 1.0f));
				spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x, y, 0)), WM::getInstance()->skin["progress"]["bar"], glm::vec2(width * fac, height), glm::vec4(0.7f, 0.7f, 0.9f, 1.0f));

				//				spriteBatch.draw(WM::getInstance()->font, text, glm::translate(matrix, glm::vec3(x,y,0)), glm::vec4(0,0,0,1));
/*				glScissor((int)shader->matrix[3][0]+1,0/*shader->height-(int)shader->matrix[3][1]-height+1*//*,width-2,1999+height-2);
				glEnable(GL_SCISSOR_TEST);
				WM::getInstance()->font->Render(text.c_str(), -1, FTPoint(x + 1.0f,-y-WM::getInstance()->font->LineHeight()-3));
				glDisable(GL_SCISSOR_TEST);*/
			}

		}
	}
}
