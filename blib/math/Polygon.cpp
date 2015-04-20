#include "Polygon.h"
#include "Line.h"
#include "Rectangle.h"
#include <limits>

#include <poly2tri/poly2tri.h>

namespace blib
{
	namespace math
	{
		bool Polygon::intersects(const Line &line) const
		{
			for (size_t i = 0; i < size(); i++)
			{
				int ii = (i + 1) % size();
				Line l2(at(i), at(ii));

				if (line.intersects(l2))
					return true;
			}
			return false;
		}
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
				push_back(p.at(i));
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

		Polygon::Polygon(const std::vector<glm::vec2> &l) : std::vector<glm::vec2>(l)
		{
			
		}

		Polygon::Polygon(const std::initializer_list<glm::vec2> &s) : std::vector<glm::vec2>(s)
		{

		}


		std::vector<p2t::Point*> Polygon::toP2TPolygon() const
		{
			std::vector<p2t::Point*> ret;
			for(size_t i = 0; i < size(); i++)
				ret.push_back(new p2t::Point(at(i)));
			return ret;		
		}

		bool Polygon::contains(glm::vec2 point) const
		{
			//http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
			int i, j, c = 0;
			for (i = 0, j = (int)size() - 1; i < (int)size(); j = i++) {
				if ((((*this)[i].y>point.y) != ((*this)[j].y > point.y)) &&
					(point.x < ((*this)[j].x - (*this)[i].x) * (point.y - (*this)[i].y) / (float)((*this)[j].y - (*this)[i].y) + (*this)[i].x))
					c = !c;
			}
			return c != 0;
		}

		glm::vec2 Polygon::projectClosest(const glm::vec2 &position) const
		{
			float minDist = 9999;
			glm::vec2 closestPoint;

			for (int i = 0; i < size(); i++)
			{
				blib::math::Line line(at(i), at((i + 1) % size()));
				glm::vec2 p = line.project(position);
				float dist = glm::distance(p, position);
				if (dist < minDist)
				{
					minDist = dist;
					closestPoint = p;
				}
			}
			return closestPoint;
		}

	}
}