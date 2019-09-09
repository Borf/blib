#include "Label.h"

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
			Label::Label( )
			{
				this->text = "";
				this->width = 100;
				this->height = 25;
			}



			void Label::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				spriteBatch.draw(WM::getInstance()->font, text, glm::translate(matrix, glm::vec3(x,y+12,0)), glm::vec4(0,0,0,1));
/*				glScissor((int)shader->matrix[3][0]+1,0/*shader->height-(int)shader->matrix[3][1]-height+1*//*,width-2,1999+height-2);
				glEnable(GL_SCISSOR_TEST);
				WM::getInstance()->font->Render(text.c_str(), -1, FTPoint(x + 1.0f,-y-WM::getInstance()->font->LineHeight()-3));
				glDisable(GL_SCISSOR_TEST);*/
			}

		}
	}
}
