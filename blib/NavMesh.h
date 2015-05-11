#pragma once

#include <blib/math/Triangle.h>

#include <vector>

namespace p2t { class Triangle; }

namespace blib
{
	class LineBatch;

	class NavTriangle : public blib::math::Triangle2
	{
	public:
		union 
		{
			NavTriangle* sides[3];
			struct
			{
				NavTriangle* t1;
				NavTriangle* t2;
				NavTriangle* t3;
			};
		};

		NavTriangle(p2t::Triangle* triangle);
		bool operator == (const NavTriangle& other);
	};


	class NavMesh
	{
	public:
		std::vector<NavTriangle> triangles;


		NavMesh(const std::vector<p2t::Triangle*> &triangles);
		void debugDraw(LineBatch& lineBatch);
		void drawPath(glm::vec2& p1, glm::vec2& p2, LineBatch& lineBatch);
	};
}