#pragma once

#include <blib/Shader.h>


namespace blib
{
	class Texture;

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





}
