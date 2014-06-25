#pragma once

#include <blib/gl/Vertex.h>
#include <vector>

namespace blib
{
	class Shapes
	{
	public:
		static std::vector<blib::VertexP3> box(const blib::VertexP3 &topleft, const blib::VertexP3 &bottomright);
		static std::vector<blib::VertexP3T2> box(const blib::VertexP3T2 &topleft, const blib::VertexP3T2 &bottomright);
	};
}