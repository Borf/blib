#include "StaticModel.h"

#include <blib/util/FileSystem.h>
#include <blib/json.h>
#include <blib/VIO.h>
#include <blib/VBO.h>
#include <blib/gl/Vertex.h>
#include <blib/RenderState.h>
#include <blib/ResourceManager.h>
#include <blib/Renderer.h>
#include <blib/Texture.h>

namespace blib
{
	StaticModel::StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer)
	{
		json::Value modelData = blib::util::FileSystem::getJson(fileName);

		std::string directory = "";
		if (fileName.find("/") != std::string::npos)
			directory = fileName.substr(0, fileName.rfind("/"));


		
		blib::VertexP3T2N3 vertex;
		float* current = &vertex.position.x;
		float* end = (&vertex.normal.z) + 1;

		//std::vector<blib::VertexP3T2N3> vertices;
		for (float f : modelData["vertices"])
		{
			*current = f;
			current++;
			if (current == end)
			{
				vertices.push_back(vertex);
				current = &vertex.position.x;
			}
			
		}
		std::vector<unsigned short> indices;
		for (json::Value& mesh : modelData["meshes"])
		{
			int start = indices.size();
			for (int i : mesh["faces"])
				indices.push_back(i);
			int end = indices.size();

			Mesh* newMesh = new Mesh();
			newMesh->material.alpha = mesh["material"]["alpha"];
			newMesh->material.diffuse = glm::vec3(mesh["material"]["diffuse"][0u], mesh["material"]["diffuse"][1u], mesh["material"]["diffuse"][2u]);
			newMesh->material.ambient = glm::vec3(mesh["material"]["ambient"][0u], mesh["material"]["ambient"][1u], mesh["material"]["ambient"][2u]);
			newMesh->material.specular = glm::vec3(mesh["material"]["specular"][0u], mesh["material"]["specular"][1u], mesh["material"]["specular"][2u]);
			newMesh->material.shinyness = mesh["material"]["shinyness"];

			if (mesh["material"]["texture"].asString() == "")
				newMesh->material.texture = NULL;
			else if (mesh["material"]["texture"].asString() != "level1/armour_table_top.jpg")
			{
				newMesh->material.texture = resourceManager->getResource<blib::Texture>(directory + "/" + mesh["material"]["texture"].asString());
				newMesh->material.texture->setTextureRepeat(true);
			}

			
			newMesh->begin = start;
			newMesh->count = end - start;
			meshes.push_back(newMesh);
		}
		vbo = resourceManager->getResource<blib::VBO>();
		vbo->setVertexFormat<blib::VertexP3T2N3>();
		renderer->setVbo(vbo, vertices);

		vio = resourceManager->getResource<blib::VIO>();
		vio->setElementType<unsigned short>();
		renderer->setVio(vio, indices);
	}



	void StaticModel::draw(RenderState& renderState, Renderer* renderer, int materialUniform)
	{

		renderState.activeVbo = vbo;
		renderState.activeVio = vio;
		
		for (auto m : meshes)
		{
			renderState.activeShader->setUniformStruct(materialUniform, m->material);
			renderState.activeTexture[0] = m->material.texture;

			renderer->drawIndexedTriangles<VertexP3T2N3>(m->begin, m->count, renderState);
		}

		renderState.activeVio = NULL;
		renderState.activeVbo = NULL;
	}


}
