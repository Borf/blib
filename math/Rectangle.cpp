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

		Rectangle::Rectangle( glm::vec2 topleft, float width, float height )
		{
			this->topleft = topleft;
			this->bottomright = topleft + glm::vec2(width, height);
		}

		void Rectangle::buildLines()
		{
			lines.push_back(blib::gl::ILineDrawable::LinePart(topleft, glm::vec2(topleft.x, bottomright.y)));
			lines.push_back(blib::gl::ILineDrawable::LinePart(glm::vec2(topleft.x, bottomright.y), bottomright));
			lines.push_back(blib::gl::ILineDrawable::LinePart(bottomright, glm::vec2(bottomright.x,topleft.y)));
			lines.push_back(blib::gl::ILineDrawable::LinePart(glm::vec2(bottomright.x,topleft.y), topleft));
		}

		bool Rectangle::contains( glm::vec2 point )
		{
			return point.x >= topleft.x && point.y >= topleft.y && point.x < bottomright.x && point.y < bottomright.y;
		}

	}
}