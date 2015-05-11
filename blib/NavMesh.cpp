#include "NavMesh.h"
#include <map>

#include <blib/LineBatch.h>
#include <blib/util/Log.h>
#include <blib/Color.h>

using blib::util::Log;


namespace blib
{




	NavMesh::NavMesh(const std::vector<p2t::Triangle*> &in)
	{
		for (auto t : in)
			triangles.push_back(t);
		Log::out << "Building navmesh connectivity" << Log::newline;

		for (NavTriangle& t1 : triangles)
		{
			for (NavTriangle& t2 : triangles)
			{
				if (t1 == t2)
					continue;

				if ((t1.v1 == t2.v1 || t1.v1 == t2.v2 || t1.v1 == t2.v3) &&
					((t1.v2 == t2.v1 || t1.v2 == t2.v2 || t1.v2 == t2.v3)))
					t1.t1 = &t2;
				if ((t1.v2 == t2.v1 || t1.v2 == t2.v2 || t1.v2 == t2.v3) &&
					((t1.v3 == t2.v1 || t1.v3 == t2.v2 || t1.v3 == t2.v3)))
					t1.t2 = &t2;
				if ((t1.v3 == t2.v1 || t1.v3 == t2.v2 || t1.v3 == t2.v3) &&
					((t1.v1 == t2.v1 || t1.v1 == t2.v2 || t1.v1 == t2.v3)))
					t1.t3 = &t2;
			}
		}
		Log::out << "Done building navmesh connectivity" << Log::newline;
	}

	void NavMesh::debugDraw(LineBatch& lineBatch)
	{
		for (NavTriangle& triangle : triangles)
		{
			lineBatch.draw(triangle);
		}
	}


	void NavMesh::drawPath(glm::vec2& p1, glm::vec2& p2, LineBatch& lineBatch)
	{
		lineBatch.draw(p1, p2, blib::Color::pink);

		NavTriangle* startTriangle = NULL;
		NavTriangle* endTriangle = NULL;

		for (NavTriangle& t : triangles)
		{
			if (t.contains(p1))
				startTriangle = &t;
			if (t.contains(p2))
				endTriangle = &t;
		}
		if (!startTriangle || !endTriangle)
			return;


		std::map<NavTriangle*, NavTriangle*> prevTriangle;
		prevTriangle[startTriangle] = NULL;
		std::list<NavTriangle*> todo;
		todo.push_back(startTriangle);

		while (!todo.empty())
		{
			NavTriangle* t = todo.front();
			todo.pop_front();

			if (t == endTriangle)
				break;

			for (NavTriangle* tt : t->sides)
			{
				if (!tt)
					continue;
				if (prevTriangle.find(tt) == prevTriangle.end())
				{
					prevTriangle[tt] = t;
					todo.push_back(tt);
				}
			}
		}


		NavTriangle* t = endTriangle;
		glm::vec2 point = p2;

		while (t)
		{
			lineBatch.draw(*t, blib::Color::orange);
			lineBatch.draw(point, t->getCenter(), blib::Color::green);


			point = t->getCenter();
			t = prevTriangle[t];
		}









	}


	NavTriangle::NavTriangle(p2t::Triangle* triangle) : math::Triangle2(triangle)
	{
		t1 = NULL;
		t2 = NULL;
		t3 = NULL;
	}

	bool NavTriangle::operator==(const NavTriangle& other)
	{
		return v1 == other.v1 && v2 == other.v2 && v3 == other.v3;
	}

}