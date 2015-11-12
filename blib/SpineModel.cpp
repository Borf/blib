#include "SpineModel.h"

#include <spine/spine.h>
#include <spine/extension.h>
#include <glm/gtc/matrix_transform.hpp>

#include <blib/util/Log.h>
#include <blib/ResourceManager.h>
#include <blib/Texture.h>
#include <blib/util/FileSystem.h>
#include <blib/gl/Vertex.h>
#include <blib/Renderer.h>

using blib::util::Log;

#define SPINE_MESH_VERTEX_COUNT_MAX 1000

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path) {
	blib::Texture* texture = blib::ResourceManager::getInstance().getResource<blib::Texture>(path);
	self->rendererObject = texture;
	self->width = texture->originalWidth;
	self->height = texture->originalHeight;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self) {
	blib::ResourceManager::getInstance().dispose((blib::Texture*)self->rendererObject);
}

char* _spUtil_readFile(const char* path, int* length) {
	char *data;
	*length = blib::util::FileSystem::getData(path, data);
	return data;
}


namespace blib
{


	SpineModel::SpineModel(const std::string &atlastFile, const std::string &skeletonFile)
	{
		atlas = spAtlas_createFromFile("assets/animations/spineboy.atlas", 0);
		json = spSkeletonJson_create(atlas);
		json->scale = 0.125f;

		skeletonData = spSkeletonJson_readSkeletonDataFile(json, "assets/animations/spineboy.json");
		if (!skeletonData) {
			Log::out << json->error << Log::newline;
			return;
		}

		skeleton = spSkeleton_create(skeletonData);
		skeleton->flipY = true;

		renderState.depthTest = false;
		renderState.blendEnabled = true;
		renderState.srcBlendColor = blib::RenderState::SRC_ALPHA;
		renderState.srcBlendAlpha = blib::RenderState::SRC_ALPHA;
		renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;

		shader = ResourceManager::getInstance().getResource<Shader>("SpriteBatch");
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_texture", 1);
		shader->bindAttributeLocation("a_color", 2);
		shader->setUniformName(ProjectionMatrix, "projectionmatrix", Shader::Mat4);
		shader->setUniformName(Matrix, "matrix", Shader::Mat4);
		shader->setUniformName(s_texture, "s_texture", Shader::Int);
		shader->finishUniformSetup();

		shader->setUniform(s_texture, 0);
		shader->setUniform(ProjectionMatrix, glm::ortho(0.0f, (float)1024.0f, (float)768.0f, 0.0f, -1000.0f, 1.0f));
		renderState.activeShader = shader;
		
		worldVertices = new float[SPINE_MESH_VERTEX_COUNT_MAX];

		stateData = spAnimationStateData_create(skeletonData);

		state = spAnimationState_create(stateData);


		spAnimationState_setAnimationByName(state, 0, "walk", true);
	}



	void SpineModel::update(double elapsedTime)
	{
		spSkeleton_update(skeleton, (float)elapsedTime);
		spAnimationState_update(state, (float)elapsedTime * 1);
		spAnimationState_apply(state, skeleton);
		spSkeleton_updateWorldTransform(skeleton);

		x += elapsedTime * 50;
		if (x > 1024+100)
			x -= 1024+200;

	}



	void SpineModel::draw(Renderer* renderer)
	{
		std::vector<blib::VertexP2T2C4> verts;
		VertexP2T2C4 vertices[4];
		VertexP2T2C4 vertex;


		for (int i = 0; i < skeleton->slotsCount; ++i) {
			spSlot* slot = skeleton->drawOrder[i];
			spAttachment* attachment = slot->attachment;
			if (!attachment) continue;


/*			switch (slot->data->blendMode) {
			case BLEND_MODE_ADDITIVE:
				blend = BlendAdd;
				break;
			case BLEND_MODE_MULTIPLY:
				blend = BlendMultiply;
				break;
			case BLEND_MODE_SCREEN: // Unsupported, fall through.
			default:
				blend = BlendAlpha;
			}
			if (states.blendMode != blend) {
				target.draw(*vertexArray, states);
				vertexArray->clear();
				states.blendMode = blend;
			}*/

			Texture* texture = 0;
			if (attachment->type == SP_ATTACHMENT_REGION) {
				spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
				texture = (Texture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
				spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

				glm::vec4 color(skeleton->r * slot->r, skeleton->g * slot->g, skeleton->b * slot->b, skeleton->a * slot->a);

				vertices[0].color = color;
				vertices[0].position.x = worldVertices[SP_VERTEX_X1];
				vertices[0].position.y = worldVertices[SP_VERTEX_Y1];
				vertices[0].texCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
				vertices[0].texCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

				vertices[1].color = color;
				vertices[1].position.x = worldVertices[SP_VERTEX_X2];
				vertices[1].position.y = worldVertices[SP_VERTEX_Y2];
				vertices[1].texCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
				vertices[1].texCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

				vertices[2].color = color;
				vertices[2].position.x = worldVertices[SP_VERTEX_X3];
				vertices[2].position.y = worldVertices[SP_VERTEX_Y3];
				vertices[2].texCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
				vertices[2].texCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

				vertices[3].color = color;
				vertices[3].position.x = worldVertices[SP_VERTEX_X4];
				vertices[3].position.y = worldVertices[SP_VERTEX_Y4];
				vertices[3].texCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
				vertices[3].texCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

				verts.push_back(vertices[0]);
				verts.push_back(vertices[1]);
				verts.push_back(vertices[2]);
				verts.push_back(vertices[0]);
				verts.push_back(vertices[2]);
				verts.push_back(vertices[3]);

			}
			else if (attachment->type == SP_ATTACHMENT_MESH) {
				spMeshAttachment* mesh = (spMeshAttachment*)attachment;
				if (mesh->verticesCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;
				spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

				glm::vec4 color(skeleton->r * slot->r, skeleton->g * slot->g, skeleton->b * slot->b, skeleton->a * slot->a);
				vertex.color = color;
				
				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoord.x = mesh->uvs[index];
					vertex.texCoord.y = mesh->uvs[index + 1];
					verts.push_back(vertex);
				}

			}
			else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
				spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
				if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;
				spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

				glm::vec4 color(skeleton->r * slot->r, skeleton->g * slot->g, skeleton->b * slot->b, skeleton->a * slot->a);
				vertex.color = color;

				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoord.x = mesh->uvs[index];
					vertex.texCoord.y = mesh->uvs[index + 1];
					verts.push_back(vertex);
				}
			}

			if (texture) {
				// SMFL doesn't handle batching for us, so we'll just force a single texture per skeleton.
				//states.texture = texture;
				renderState.activeTexture[0] = texture;
			}
		}

		shader->setUniform(Matrix, glm::translate(glm::mat4(), glm::vec3(x,768,0)));

		renderState.cullFaces = RenderState::CullFaces::NONE;
		/*verts.clear();

		verts.push_back(VertexP2T2C4(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec4(1, 1, 1, 1)));
		verts.push_back(VertexP2T2C4(glm::vec2(500, 0), glm::vec2(1, 0), glm::vec4(1, 1, 1, 1)));
		verts.push_back(VertexP2T2C4(glm::vec2(0, 500), glm::vec2(0, 1), glm::vec4(1, 1, 1, 1)));*/

		renderer->drawTriangles(verts, renderState);
	}

}