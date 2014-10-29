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
	public:
		StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer);


		void draw(RenderState& renderState, Renderer* renderer);
	};

}