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

#include <map>

namespace blib
{
	static glm::mat4 jsonToMatrix(const blib::json::Value &v)
	{
		glm::mat4 mat;

		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				mat[y][x] = v[y][x];
		return mat;
	}



	StaticModel::StaticModel(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer)
	{
		json::Value modelData = blib::util::FileSystem::getJson(fileName);

		std::string directory = "";
		if (fileName.find("/") != std::string::npos)
			directory = fileName.substr(0, fileName.rfind("/"));


#if 1
		blib::VertexP3T2N3 vertex;
		float* current = &vertex.position.x;
		float* end = (&vertex.normal.z) + 1;
#else
		blib::VertexP3T2N3B4B4 vertex;
		float* current = &vertex.position.x;
		float* end = (&vertex.boneWeights.w) + 1;
#endif

		//std::vector<blib::VertexP3T2N3> vertices;
		int i = 0;
		for (float f : modelData["vertices"])
		{
			if (i >= 8 && i < 8 + 4)
				*((int*)current) = (int)f;
			else
				*current = f;
			current++;
			i++;
			if (current == end)
			{
				i = 0;
				vertices.push_back(vertex);
				current = &vertex.position.x;
			}
			
		}
		std::vector<unsigned short> indices;
		for (const json::Value& mesh : modelData["meshes"])
		{
			int start = indices.size();
			for (int i : mesh["faces"])
				indices.push_back(i);
			int end = indices.size();

			Mesh* newMesh = new Mesh();
			newMesh->material.alpha = mesh["material"]["alpha"];
			newMesh->material.diffuse = glm::vec3(mesh["material"]["diffuse"][0], mesh["material"]["diffuse"][1], mesh["material"]["diffuse"][2]);
			newMesh->material.ambient = glm::vec3(mesh["material"]["ambient"][0], mesh["material"]["ambient"][1], mesh["material"]["ambient"][2]);
			newMesh->material.specular = glm::vec3(mesh["material"]["specular"][0], mesh["material"]["specular"][1], mesh["material"]["specular"][2]);
			newMesh->material.shinyness = mesh["material"]["shinyness"];

			if (mesh["material"]["texture"].asString() == "")
				newMesh->material.texture = NULL;
			else //if (mesh["material"]["texture"].asString() != "level1/armour_table_top.jpg")
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

		triangles.reserve(indices.size() / 3);
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			triangles.push_back(blib::math::Triangle3(
				vertices[indices[i]].position,
				vertices[indices[i+1]].position,
				vertices[indices[i+2]].position
				));
		}

	}



	void StaticModel::draw(RenderState& renderState, Renderer* renderer, const std::function<void(const Material& material)> &materialCallback)
	{
		renderState.activeVbo = vbo;
		renderState.activeVio = vio;
		
		for (auto m : meshes)
		{
			if (materialCallback)
				materialCallback(m->material);
			renderer->drawIndexedTriangles<VertexP3T2N3>(m->begin, m->count, renderState);
		}

		renderState.activeVio = NULL;
		renderState.activeVbo = NULL;
	}


}
