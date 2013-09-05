#pragma once

#include <glm/glm.hpp>
#include <list>


namespace blib
{
	class IDrawableLine
	{
	public:
		class LinePart { public: glm::vec2 p1, p2; LinePart(glm::vec2 p1, glm::vec2 p2) : p1(p1), p2(p2) { } };
		std::list<LinePart> lines;
		virtual void buildLines() = 0;
		inline std::list<LinePart> &getLines()
		{
			if(lines.empty())
				buildLines();
			return lines;
		}
		inline void resetLines()
		{
			lines.clear();
		}
	};
}