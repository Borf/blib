#pragma once

#include <blib/RenderState.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct spAtlas;
struct spSkeletonData;
struct spSkeletonJson;
struct spSkeleton;
struct spAnimationState;
struct spAnimationStateData;

namespace blib
{
	class Renderer;
	class SpriteBatch;
	class SpineModel;

	class SpineModelInstance
	{
	public:
		SpineModel* model;
		spSkeleton* skeleton;
		spAnimationState* state;

		void update(double elapsedTime);
		void draw(const glm::mat4& transform, SpriteBatch& spriteBatch);

		void playAnimation(const std::string &name, bool loop);
		void stopAnimation(const std::string &name);
	};

	class SpineModel
	{
		spAtlas* atlas;
		spSkeletonJson* json;
		spSkeletonData *skeletonData;
		spAnimationStateData* stateData;

		std::vector<SpineModelInstance*> instances;

	public:
		SpineModel(const std::string &atlastFile, const std::string &skeletonFile);
		~SpineModel();

		SpineModelInstance* getInstance();
		void disposeInstance(SpineModelInstance* instance);
	};


}