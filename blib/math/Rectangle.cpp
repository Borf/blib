#include "Rectangle.h"

#ifdef CLIPPER
#include <clipper/clipper.hpp>
#endif

#ifdef POLY2TRI
#include <poly2tri/poly2tri.h>
#endif

#include <blib/math/Line.h>
#include <blib/linq.h>

namespace blib
{
	namespace math
	{


		Rectangle::Rectangle( float x, float y, float width, float height )
		{
			topleft = glm::vec2(x,y);
			bottomright = topleft + glm::vec2(width, height);
		}
		Rectangle::Rectangle( int x, int y, int width, int height )
		{
			topleft = glm::vec2(x,y);
			bottomright = topleft + glm::vec2(width, height);
		}

		Rectangle::Rectangle( glm::vec2 topleft, float width, float height )
		{
			this->topleft = topleft;
			this->bottomright = topleft + glm::vec2(width, height);
		}
		Rectangle::Rectangle( glm::vec2 topleft, int width, int height )
		{
			this->topleft = topleft;
			this->bottomright = topleft + glm::vec2(width, height);
		}

		Rectangle::Rectangle( glm::vec2 topleft, glm::vec2 bottomright )
		{
			this->topleft = topleft;
			this->bottomright = bottomright;
		}
		Rectangle::Rectangle(const Rectangle & rectangle)
		{
			this->topleft = rectangle.topleft;
			this->bottomright = rectangle.bottomright;
		}

		void Rectangle::buildLines()
		{
			lines.push_back(blib::IDrawableLine::LinePart(topleft, glm::vec2(topleft.x, bottomright.y)));
			lines.push_back(blib::IDrawableLine::LinePart(glm::vec2(topleft.x, bottomright.y), bottomright));
			lines.push_back(blib::IDrawableLine::LinePart(bottomright, glm::vec2(bottomright.x,topleft.y)));
			lines.push_back(blib::IDrawableLine::LinePart(glm::vec2(bottomright.x,topleft.y), topleft));
		}

		bool Rectangle::contains(const glm::vec2 &point) const
		{
			return point.x >= topleft.x && point.y >= topleft.y && point.x <= bottomright.x && point.y <= bottomright.y;
		}		

		bool Rectangle::containsX(const glm::vec2 &point) const
		{
			return point.x > topleft.x && point.x < bottomright.x;
		}

#ifdef clipper_hpp
		ClipperLib::Polygon Rectangle::toClipperPolygon() const
		{
			ClipperLib::Polygon ret;
			ret.push_back(topleft);
			ret.push_back(glm::vec2(topleft.x, bottomright.y));
			ret.push_back(bottomright);
			ret.push_back(glm::vec2(bottomright.x, topleft.y));
			return ret;
		}
#endif

#ifdef POLY2TRI_H
		std::vector<p2t::Point*> Rectangle::toP2TPolygon() const
		{
			std::vector<p2t::Point*> ret;
			ret.push_back(new p2t::Point(topleft));
			ret.push_back(new p2t::Point(glm::vec2(topleft.x, bottomright.y)));
			ret.push_back(new p2t::Point(bottomright));
			ret.push_back(new p2t::Point(glm::vec2(bottomright.x, topleft.y)));
			return ret;
		}
#endif

		bool Rectangle::intersect( const blib::math::Rectangle &other ) const
		{
			return other.contains(topleft) || other.contains(bottomright) || other.contains(glm::vec2(topleft.x, bottomright.y)) || other.contains(glm::vec2(bottomright.x, topleft.y)) ||
				contains(other.topleft) || contains(other.bottomright) || contains(glm::vec2(other.topleft.x, other.bottomright.y)) || contains(glm::vec2(other.bottomright.x, other.topleft.y));
		}

		const int INSIDE = 0; // 0000
		const int LEFT = 1;   // 0001
		const int RIGHT = 2;  // 0010
		const int BOTTOM = 4; // 0100
		const int TOP = 8;    // 1000
		int Rectangle::computeOutCode(const glm::vec2 &p) const
		{
			int code;

			code = INSIDE;          // initialised as being inside of clip window

			if (p.x < topleft.x)           // to the left of clip window
				code |= LEFT;
			else if (p.x > bottomright.x)      // to the right of clip window
				code |= RIGHT;
			if (p.y < topleft.y)           // below the clip window
				code |= BOTTOM;
			else if (p.y > bottomright.y)      // above the clip window
				code |= TOP;

			return code;
		}


		//http://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
		bool Rectangle::intersect(blib::math::Line l) const
		{
			// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
			int outcode0 = computeOutCode(l.p1);
			int outcode1 = computeOutCode(l.p2);
			bool accept = false;
			while (true) 
			{
				if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
					accept = true;
					break;
				}
				else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
					break;
				}
				else {
					// failed both tests, so calculate the line segment to clip
					// from an outside point to an intersection with clip edge
					float x, y;

					// At least one endpoint is outside the clip rectangle; pick it.
					int outcodeOut = outcode0 ? outcode0 : outcode1;

					// Now find the intersection point;
					// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
					if (outcodeOut & TOP) {           // point is above the clip rectangle
						x = l.p1.x + (l.p2.x - l.p1.x) * (bottomright.y - l.p1.y) / (l.p2.y - l.p1.y);
						y = bottomright.y;
					}
					else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
						x = l.p1.x + (l.p2.x - l.p1.x) * (topleft.y - l.p1.y) / (l.p2.y - l.p1.y);
						y = topleft.y;
					}
					else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
						y = l.p1.y + (l.p2.y - l.p1.y) * (bottomright.x - l.p1.x) / (l.p2.x - l.p1.x);
						x = bottomright.x;
					}
					else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
						y = l.p1.y + (l.p2.y - l.p1.y) * (topleft.x - l.p1.x) / (l.p2.x - l.p1.x);
						x = topleft.x;
					}

					// Now we move outside point to intersection point to clip
					// and get ready for next pass.
					if (outcodeOut == outcode0) {
						l.p1 = glm::vec2(x, y);
						outcode0 = computeOutCode(l.p1);
					}
					else {
						l.p2 = glm::vec2(x, y);
						outcode1 = computeOutCode(l.p2);
					}
				}
			}
			return accept;

		}

		void Rectangle::inflate( float x, float y )
		{
			topleft.x -= x;
			topleft.y -= y;
			bottomright.x += x;
			bottomright.y += y;
		}

		Rectangle Rectangle::operator*(const glm::mat4 &matrix)
		{
			return Rectangle(glm::vec2(matrix * glm::vec4(topleft, 0, 1)), glm::vec2(matrix * glm::vec4(bottomright, 0, 1)));
		}
		void Rectangle::operator*=(const glm::mat4 &matrix)
		{
			topleft = glm::vec2(matrix * glm::vec4(topleft, 0, 1));
			bottomright = glm::vec2(matrix * glm::vec4(bottomright, 0, 1));
		}
		void Rectangle::operator*=(float scalar)
		{
			topleft *= scalar;
			bottomright *= scalar;
		}

		Rectangle Rectangle::operator+(const glm::vec2 &offset)
		{
			return Rectangle(topleft + offset, bottomright + offset);
		}
		Rectangle Rectangle::operator*(float scalar)
		{
			return Rectangle(topleft * scalar, bottomright * scalar);
		}

		float Rectangle::overlap(const Rectangle &other) const
		{
			glm::vec2 o = glm::max(glm::vec2(0,0), glm::min(bottomright, other.bottomright) - glm::max(topleft, other.topleft));
			return o.x * o.y;
		}

		void Rectangle::moveBy(const glm::vec2 &offset)
		{
			topleft += offset;
			bottomright += offset;
		}

		glm::vec2 Rectangle::projectClosest(const glm::vec2 &position) const
		{
			std::vector<glm::vec2> points(4);
			points[0] = blib::math::Line(topleft, glm::vec2(topleft.x, bottomright.y)).project(position);
			points[1] = blib::math::Line(glm::vec2(topleft.x, bottomright.y), bottomright).project(position);
			points[2] = blib::math::Line(bottomright, glm::vec2(bottomright.x, topleft.y)).project(position);
			points[3] = blib::math::Line(glm::vec2(bottomright.x, topleft.y), topleft).project(position);

			return blib::linq::min<float, glm::vec2>(points, [position](glm::vec2 p){ return glm::distance(position, p);  }, [](glm::vec2 p){ return p; });





		}

	}
}