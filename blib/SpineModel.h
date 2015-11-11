#pragma once

#include <blib/RenderState.h>
#include <string>

struct spAtlas;
struct spSkeletonData;
struct spSkeletonJson;
struct spSkeleton;
struct spAnimationState;
struct spAnimationStateData;

namespace blib
{
	class Renderer;

	class SpineModel
	{
		spAtlas* atlas;
		spSkeletonJson* json;
		spSkeletonData *skeletonData;
		spSkeleton* skeleton;
		spAnimationState* state;
		spAnimationStateData* stateData;

		RenderState renderState;
		blib::Shader* shader;
		enum ShaderAttributes
		{
			ProjectionMatrix,
			Matrix,
			s_texture,
		};

		float* worldVertices;

		float x = 0;

	public:
		SpineModel(const std::string &atlastFile, const std::string &skeletonFile);

		void update(double elapsedTime);
		void draw(Renderer* renderer);
	};


}