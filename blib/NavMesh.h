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

	class NavMesh;
	class Actor
	{
	public:
		glm::vec2 position;
		std::vector<glm::vec2> route;
		int index;
		NavMesh* navMesh;

		Actor(NavMesh* navMesh, const glm::vec2& position);
		void calculateRoute(const glm::vec2 &target);
		glm::vec2 update(float elapsedTime);
	};

	class NavMesh
	{
	public:
		std::vector<NavPoly> polygons;
		
		std::vector<blib::math::Polygon> concavePolygons;

		NavMesh(const std::vector<p2t::Triangle*> &triangles);
		void debugDraw(LineBatch& lineBatch);
		std::vector<glm::vec2> getPath(const glm::vec2& p1, const glm::vec2& p2);
		bool canWalk(const blib::math::Line& line);
	};
}