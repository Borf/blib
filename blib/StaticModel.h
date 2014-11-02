#pragma once

#include <string>
#include <vector>
#include <blib/Shader.h>
#include <blib/gl/Vertex.h>

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
		};



		VBO* vbo;
		VIO* vio;
		std::vector<Mesh*> meshes;

		std::vector<blib::VertexP3T2N3> vertices;

	public:
		StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer);


		void draw(RenderState& renderState, Renderer* renderer, int materialUniform);
	};

}