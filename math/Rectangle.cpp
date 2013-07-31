#include "Rectangle.h"

#include <poly2tri/poly2tri.h>

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

		Rectangle::Rectangle( glm::vec2 topleft, glm::vec2 bottomright )
		{
			this->topleft = topleft;
			this->bottomright = bottomright;
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

		ClipperLib::Polygon Rectangle::toClipperPolygon() const
		{
			ClipperLib::Polygon ret;
			ret.push_back(topleft);
			ret.push_back(glm::vec2(topleft.x, bottomright.y));
			ret.push_back(bottomright);
			ret.push_back(glm::vec2(bottomright.x, topleft.y));
			return ret;
		}

		std::vector<p2t::Point*> Rectangle::toP2TPolygon() const
		{
			std::vector<p2t::Point*> ret;
			ret.push_back(new p2t::Point(topleft));
			ret.push_back(new p2t::Point(glm::vec2(topleft.x, bottomright.y)));
			ret.push_back(new p2t::Point(bottomright));
			ret.push_back(new p2t::Point(glm::vec2(bottomright.x, topleft.y)));
			return ret;
		}

	}
}