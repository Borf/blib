#include "NavMesh.h"
#include <map>

#include <blib/math/Line.h>
#include <blib/LineBatch.h>
#include <blib/util/Log.h>
#include <blib/Color.h>
#include <blib/linq.h>
#include <blib/math/Rectangle.h>
#include <poly2tri/poly2tri.h>

using blib::util::Log;


namespace blib
{




	NavMesh::NavMesh(const std::vector<p2t::Triangle*> &in)
	{
		std::vector<bool> used(in.size(), false);

		for (size_t i = 0; i < in.size(); i++)
		{
			if (used[i])
				continue;
			used[i] = true;
			NavPoly p(in[i]);

			bool changed = true;
			while (changed)
			{
				changed = false;
				for (size_t ii = 0; ii < in.size(); ii++)
				{
					if (used[ii])
						continue;

					if (p.containsEdge(blib::math::Line(glm::vec2(in[ii]->GetPoint(0)->x, in[ii]->GetPoint(0)->y), glm::vec2(in[ii]->GetPoint(1)->x, in[ii]->GetPoint(1)->y))) ||
						p.containsEdge(blib::math::Line(glm::vec2(in[ii]->GetPoint(1)->x, in[ii]->GetPoint(1)->y), glm::vec2(in[ii]->GetPoint(2)->x, in[ii]->GetPoint(2)->y))) ||
						p.containsEdge(blib::math::Line(glm::vec2(in[ii]->GetPoint(2)->x, in[ii]->GetPoint(2)->y), glm::vec2(in[ii]->GetPoint(0)->x, in[ii]->GetPoint(0)->y))))
					{
						NavPoly p2 = p;
						p2.add(in[ii]);
						if (p2.isConvex())
						{
							p = p2;
							used[ii] = true;
							changed = true;
						}
					}
				}
			}
			polygons.push_back(p);

		}

		ClipperLib::Clipper clipper;

		ClipperLib::Polygons raw;

		for (auto t : in)
			raw.push_back(blib::math::Polygon(t).toClipperPolygon());
		
		ClipperLib::Polygons offsetted;
		ClipperLib::OffsetPolygons(raw, offsetted, 500);

		ClipperLib::Polygons out;
		clipper.AddPolygons(offsetted, ClipperLib::ptSubject);
		clipper.Execute(ClipperLib::ctUnion, out);
		concavePolygons.clear();
		for (auto t : out)
			concavePolygons.push_back(t);




		
		Log::out << "Building navmesh connectivity" << Log::newline;

		for (NavPoly& t1 : polygons)
		{
			for (NavPoly& t2 : polygons)
			{
				if (t1 == t2)
					continue;
				
				for (int i = 0; i < (int)t1.size(); i++)
				{
					if (t2.containsEdge(t1.getLine(i)))
						t1.connected.push_back(&t2);
				}
			}
		}
		Log::out << "Done building navmesh connectivity" << Log::newline;
		




	}

	void NavMesh::debugDraw(LineBatch& lineBatch)
	{
		for (NavPoly& poly : polygons)
		{
			lineBatch.draw(poly);
		}
	}


	std::vector<glm::vec2> NavMesh::getPath(const glm::vec2& p1, const glm::vec2& p2)
	{
		NavPoly* startPoly = NULL;
		NavPoly* endPoly = NULL;

		for (NavPoly& t : polygons)
		{
			if (t.contains(p1))
				startPoly = &t;
			if (t.contains(p2))
				endPoly = &t;
		}
		if (!startPoly || !endPoly)
			return std::vector<glm::vec2>();

		std::map<NavPoly*, NavPoly*> prevTriangle;
		prevTriangle[startPoly] = NULL;
		std::list<NavPoly*> todo;
		todo.push_back(startPoly);

		while (!todo.empty())
		{
			NavPoly* t = todo.front();
			todo.pop_front();

			if (t == endPoly)
				break;

			for (NavPoly* tt : t->connected)
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


		std::vector<glm::vec2> route;
		route.push_back(p2);
		NavPoly* t = endPoly;

		while (prevTriangle[t])
		{
			NavPoly* next = prevTriangle[t];
			glm::vec2 avg;
			int avgCount = 0;
			for (glm::vec2 p : *t)
			{
				if (blib::linq::containsValue(*next, p))
				{
					avg += p;
					avgCount++;
				}
			}
			route.push_back(avg / (float)avgCount);
			t = next;
		}
		route.push_back(p1);
		std::reverse(route.begin(), route.end());
		return route;
	}

	bool NavMesh::canWalk(const blib::math::Line& line)
	{
		for (const blib::math::Polygon& p : concavePolygons)
			if (p.intersects(line))
				return false;
		return true;
	}




	NavPoly::NavPoly(p2t::Triangle* t)
	{
		for (int i = 0; i < 3; i++)
			push_back(glm::vec2(t->GetPoint(i)->x, t->GetPoint(i)->y));
	}

	blib::math::Line NavPoly::getLine(int index) const
	{
		int next = (index + 1) % size();
		return blib::math::Line(at(index), at(next));
	}

	bool NavPoly::containsEdge(const blib::math::Line& line) const
	{
		for (size_t i = 0; i < size(); i++)
		{
			size_t next = (i + 1) % size();
			if ((line.p1 == at(i) && line.p2 == at(next)) ||
				(line.p2 == at(i) && line.p1 == at(next)))
				return true;
		}
		return false;
	}



	Actor::Actor(NavMesh* navMesh, const glm::vec2& position)
	{
		this->navMesh = navMesh;
		this->position = position;
		index = 0;
		navMesh = NULL;
	}

	void Actor::calculateRoute(const glm::vec2 &target)
	{
		route = navMesh->getPath(position, target);
		index = 0;
	}

	glm::vec2 Actor::update(float elapsedTime)
	{
		while (index+1 < (int)route.size())
		{
			if (!navMesh->canWalk(blib::math::Line(position, route[index + 1])))
				break;
			index++;
		}

		glm::vec2 target = route[index];

		glm::vec2 diff = glm::normalize(target - position) * elapsedTime * 100.0f;

		position += diff;


		return position;
	}

}