#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <blib/IDrawableLine.h>

#include <clipper/clipper.hpp>

namespace p2t { struct Point; class Triangle; }

namespace blib
{
	namespace math
	{
		class Line;
		class Rectangle;

		class Polygon : public std::vector<glm::vec2>, public blib::IDrawableLine
		{
		public:
			Polygon() { }
			Polygon(const ClipperLib::Polygon &p);
			Polygon(const std::vector<glm::vec2> &l);
			Polygon(const std::initializer_list<glm::vec2> &s);
			ClipperLib::Polygon toClipperPolygon() const;
			Polygon(p2t::Triangle *t);


			std::vector<p2t::Point*> toP2TPolygon() const;
			bool contains(glm::vec2 point) const;
			bool intersects(const Line& line) const;
			bool intersects(const Line& line, glm::vec2 &point, Line &collidedLine) const;
			bool intersects( const Line &line, std::vector<std::pair<glm::vec2, Line> >* collisions) const;

			glm::vec2 normal(int index) const;

			blib::math::Rectangle getBoundingBox() const;

			glm::vec2 projectClosest(const glm::vec2 &position) const;
			void buildLines();

			void push_back(const value_type& _Val)
			{
				std::vector<glm::vec2>::push_back(_Val);
				lines.clear();
			}

#ifdef WIN32
			void push_back(value_type&& _Val)
			{
				std::vector<glm::vec2>::push_back(_Val);
				lines.clear();
			}
#endif

			void clear()
			{
				std::vector<glm::vec2>::clear();
				lines.clear();
			}

		};

	}
}
