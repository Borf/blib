#include "Polygon.h"
#include "Line.h"


namespace blib
{
	namespace math
	{
		bool Polygon::intersects( const Line &line, glm::vec2 &point, Line &collidedLine)
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

		glm::vec2 Polygon::normal(int index)
		{
			int index2 = (index+1)%size();
			return glm::normalize(glm::vec2(at(index2).y - at(index).y, -(at(index2).x - at(index).x)));
		}


		void Polygon::buildLines()
		{
			for(size_t i = 0; i < size(); i++)
			{
				int ii = (i+1)%size();
				lines.push_back(blib::gl::ILineDrawable::LinePart(at(i), at(ii)));
			}
		}

	}
}