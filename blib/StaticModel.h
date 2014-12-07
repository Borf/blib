#pragma once

#include <string>
#include <vector>
#include <blib/Shader.h>
#include <blib/gl/Vertex.h>
#include <blib/math/Triangle.h>
#include <blib/json.h>
#include <blib/util/Tree.h>

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
	public:
		class Material : public blib::Shader::UniformStruct<Material>
		{
		public:
			glm::vec3 diffuse;
			glm::vec3 ambient;
			glm::vec3 specular;
			float shinyness;
			float alpha;

			Texture* texture;

			Material()
			{
				reg(diffuse, "diffuse");
				reg(ambient, "ambient");
				reg(specular, "specular");
				reg(shinyness, "shinyness");
				reg(alpha, "alpha");
				texture = NULL;
			}

			static void init()
			{
				regStatic<glm::vec3>("diffuse");
				regStatic<glm::vec3>("ambient");
				regStatic<glm::vec3>("specular");
				regStatic<float>("shinyness");
				regStatic<float>("alpha");
			}
		};
	private:
		class Mesh
		{
		public:
			Material material;
			int begin;
			int count;
			blib::json::Value jsonData;

		};
	public:
		class Bone : public blib::util::DynTree < Bone >
		{
		public:
			std::string name;
			glm::mat4 matrix;
			glm::mat4 animMatrix;

//			glm::mat4 origin;
			Bone* parent;

			bool isBone;
		};

		Bone* rootBone;

		std::vector<blib::math::Triangle3> triangles;
	public:
		const std::vector<blib::math::Triangle3>& getTriangles() const { return triangles; }


		VBO* vbo;
		VIO* vio;
		std::vector<Mesh*> meshes;

		std::vector<blib::VertexP3T2N3> vertices;


		blib::json::Value jsonData;

		blib::json::Value animationData;

	public:
		StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer);
		void draw(RenderState& renderState, Renderer* renderer, int materialUniform);
	};

}