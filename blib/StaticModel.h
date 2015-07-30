#pragma once

#include <string>
#include <vector>
#include <functional>

#include <blib/Shader.h>
#include <blib/gl/Vertex.h>
#include <blib/math/Triangle.h>
#include <blib/json.h>
#include <blib/util/Tree.h>
#include <blib/Material.h>


namespace blib
{
	class VBO;
	class VIO;
	class Renderer;
	class ResourceManager;
	class RenderState;
	class Texture;

	class StaticModel
	{
	private:
		class Mesh
		{
		public:
			Material material;
			int begin;
			int count;
		};
	public:
		std::vector<blib::math::Triangle3> triangles;
	public:
		const std::vector<blib::math::Triangle3>& getTriangles() const { return triangles; }


		VBO* vbo;
		VIO* vio;
		std::vector<Mesh*> meshes;

		std::vector<blib::VertexP3T2N3> vertices;
	public:
		StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer);
		void draw(RenderState& renderState, Renderer* renderer, const std::function<void(const Material& material)> &materialCallback = nullptr);
	};

}