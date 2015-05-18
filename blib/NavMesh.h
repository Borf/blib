#pragma once

#include <blib/math/Triangle.h>
#include <blib/math/Polygon.h>

#include <vector>

namespace p2t { class Triangle; }

namespace blib
{
	class LineBatch;

	class NavPoly : public blib::math::Polygon
	{
	public:
		std::vector<NavPoly*> connected;


		NavPoly(p2t::Triangle* triangle);
		bool containsEdge(const blib::math::Line& line) const;
		blib::math::Line getLine(int index) const;
	};


	class NavMesh
	{
	public:
		std::vector<NavPoly> polygons;
		
		std::vector<blib::math::Polygon> concavePolygons;

		NavMesh(const std::vector<p2t::Triangle*> &triangles);
		void debugDraw(LineBatch& lineBatch);
		void drawPath(glm::vec2& p1, glm::vec2& p2, LineBatch& lineBatch);
		bool canWalk(const blib::math::Line& line);
	};
}