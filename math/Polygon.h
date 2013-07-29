#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <blib/gl/LineBatch.h>
#include <clipper/clipper.hpp>

namespace blib
{
	namespace math
	{
		class Line;

		class Polygon : public std::vector<glm::vec2>, public blib::gl::ILineDrawable
		{
		public:
			Polygon() { }
			Polygon(const ClipperLib::Polygon &p);


			ClipperLib::Polygon toClipperPolygon() const;
			bool contains(glm::vec2 point) const;
			bool intersects(const Line& line) const;
			bool intersects(const Line& line, glm::vec2 &point, Line &collidedLine) const;
			bool intersects( const Line &line, std::vector<std::pair<glm::vec2, Line> >* collisions) const;

			glm::vec2 normal(int index) const;


			void buildLines();

			void push_back(const value_type& _Val)
			{
				std::vector<glm::vec2>::push_back(_Val);
				lines.clear();
			}

			void push_back(value_type&& _Val)
			{
				std::vector<glm::vec2>::push_back(_Val);
				lines.clear();
			}

			void clear()
			{
				std::vector<glm::vec2>::clear();
				lines.clear();
			}

		};

	}
}
