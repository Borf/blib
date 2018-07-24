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
				spriteBatch.draw(WM::getInstance()->font, text, glm::translate(matrix, glm::vec3(x,y,0)), glm::vec4(0,0,0,1));
			}

		}
	}
}
