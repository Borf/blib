#include "Rectangle.h"

namespace blib
{
	namespace math
	{


		Rectangle::Rectangle( float x, float y, float width, float height )
		{
			topleft = glm::vec2(x,y);
			bottomright = topleft + glm::vec2(width, height);
		}
	}
}