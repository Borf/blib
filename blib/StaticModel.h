#pragma once

#include <string>
#include <vector>

#include <blib/gl/Vertex.h>

namespace blib
{
	class VBO;
	class VIO;
	class Renderer;
	class ResourceManager;
	class RenderState;

	class StaticModel
	{
		VBO* vbo;
		VIO* vio;

		std::vector<blib::VertexP3T2N3> vertices; // temporary for copying constructor -> draw
		std::vector<unsigned short> indices; // temporary for copying constructor -> draw

	public:
		StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer);


		void draw(RenderState& renderState, Renderer* renderer);
	};

}