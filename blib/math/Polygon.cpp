#include "Polygon.h"
#include "Line.h"
#include "Rectangle.h"
#include <limits>

#include <poly2tri/poly2tri.h>

namespace blib
{
	namespace math
	{
		bool Polygon::intersects( const Line &line, glm::vec2 &point, Line &collidedLine) const
		{
			for(size_t i = 0; i < size(); i++)
			{
				int ii = (i+1)%size();
				Line l2(at(i), at(ii));

				if(line.intersects(l2, point))
				{
					collidedLine.p1 = l2.p1;
					collidedLine.p2 = l2.p2;
					return true;
				}
			}
			return false;
		}

		bool Polygon::intersects( const Line &line, std::vector<std::pair<glm::vec2, Line> >* collisions) const
		{
			glm::vec2 point;
			bool collision = false;
			for(size_t i = 0; i < size(); i++)
			{
				int ii = (i+1)%size();
				Line l2(at(i), at(ii));

				if(line.intersects(l2, point))
				{
					if(collisions != NULL)
						collisions->push_back(std::pair<glm::vec2, Line>(point, l2));
					collision = true;
				}
			}
			return collision;
		}


		glm::vec2 Polygon::normal(int index) const
		{
			int index2 = (index+1)%size();
			return glm::normalize(glm::vec2(at(index2).y - at(index).y, -(at(index2).x - at(index).x)));
		}


		void Polygon::buildLines()
		{
			for(size_t i = 0; i < size(); i++)
			{
				int ii = (i+1)%size();
				lines.push_back(blib::IDrawableLine::LinePart(at(i), at(ii)));
			}
		}

		ClipperLib::Polygon Polygon::toClipperPolygon() const
		{
			ClipperLib::Polygon ret;
			for(size_t i = 0; i < size(); i++)
				ret.push_back(at(i));
			return ret;
		}

		Polygon::Polygon( const ClipperLib::Polygon &p )
		{
			reserve(p.size());
			for(size_t i = 0; i < p.size(); i++)
				push_back(p.at(i)());
		}

		blib::math::Rectangle Polygon::getBoundingBox() const
		{
			blib::math::Rectangle ret(glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), glm::vec2(std::numeric_limits<float>::min(), std::numeric_limits<float>::min()));

			for(size_t i = 0; i < size(); i++)
			{
				ret.topleft = glm::min(ret.topleft, at(i));
				ret.bottomright = glm::max(ret.bottomright, at(i));
			}
			return ret;
		}

		Polygon::Polygon( p2t::Triangle *t)
		{
			for(int i = 0; i < 3; i++)
				push_back(glm::vec2(t->GetPoint(i)->x, t->GetPoint(i)->y));
		}


		std::vector<p2t::Point*> Polygon::toP2TPolygon() const
		{
			std::vector<p2t::Point*> ret;
			for(size_t i = 0; i < size(); i++)
				ret.push_back(new p2t::Point(at(i)));
			return ret;		
		}

	}
}