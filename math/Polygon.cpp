#include "Polygon.h"
#include "Line.h"


namespace blib
{
	namespace math
	{
		bool Polygon::intersects( const Line &line, glm::vec2 &point, glm::vec2 &normal )
		{
			for(size_t i = 0; i < size(); i++)
			{
				int ii = (i+1)%size();
				Line l2(at(i), at(ii));

				if(line.intersects(l2))
				{
					return true;
				}

			}


			return false;
		}

	}
}