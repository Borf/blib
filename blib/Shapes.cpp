#include "Shapes.h"



namespace blib
{

	std::vector<blib::VertexP3> Shapes::box(const blib::VertexP3 &tl, const blib::VertexP3 &br)
	{
		std::vector<blib::VertexP3> ret;
		//front
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, tl.position.z)));

		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, tl.position.z)));
		//back
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, br.position.z)));

		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, br.position.z)));

		//top
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, br.position.z)));

		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, br.position.z)));
		//bottom
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, br.position.z)));

		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, br.position.z)));

		//left
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, br.position.z)));

		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(tl.position.x, tl.position.y, br.position.z)));
		//right
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, br.position.z)));

		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, br.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, br.position.y, tl.position.z)));
		ret.push_back(blib::VertexP3(glm::vec3(br.position.x, tl.position.y, br.position.z)));


		return ret;
	}

//todo: finish texcoords here
	std::vector<blib::VertexP3T2> Shapes::box(const blib::VertexP3T2 &tl, const blib::VertexP3T2 &br)
	{
		std::vector<blib::VertexP3T2> ret;
		//front
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, tl.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, br.texCoord.y)));

		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, tl.position.z), glm::vec2(br.texCoord.x, br.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, tl.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, br.texCoord.y)));
		//back
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, br.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, br.texCoord.y)));

		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, br.position.z), glm::vec2(br.texCoord.x, br.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, br.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, br.texCoord.y)));

		//top
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, tl.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, br.texCoord.y)));

		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, br.position.z), glm::vec2(br.texCoord.x, br.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, tl.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, br.texCoord.y)));
		//bottom
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, tl.position.z), glm::vec2(br.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));

		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));

		//left
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));

		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(tl.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		//right
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));

		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, br.position.y, tl.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));
		ret.push_back(blib::VertexP3T2(glm::vec3(br.position.x, tl.position.y, br.position.z), glm::vec2(tl.texCoord.x, tl.texCoord.y)));


		return ret;
	}

}