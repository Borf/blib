#pragma once

#include <blib/RenderState.h>
#include <string>
#include <vector>
#include <functional>
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
	namespace math { class Polygon; }

	class SpineModelInstance
	{
	public:
		std::string lastAnimation;
		float fixedTime = -1;
		SpineModel* model;
		spSkeleton* skeleton;
		spAnimationState* state;
		std::function<void()> callback = nullptr;

		void update(double elapsedTime);
		void draw(const glm::mat4& transform, SpriteBatch& spriteBatch);

		void playAnimation(const std::string &name, bool loop);
		void playAnimationOnTrack(const std::string &name, int track, bool loop);
		void playAnimation(const std::string &name, const std::function<void()> &callback);
		void stopAnimation(const std::string &name, bool reset = true);
		void stopAnimationOnTrack(const std::string &name, int track);

		void setAnimation(const std::string &name, float time);
		
		void setAttachment(const std::string &slot, const std::string &attachment);
		float skeletonTime() const;
		bool isPlaying();
		blib::math::Polygon getPart(const std::string &name);
		std::pair<blib::Texture*, std::vector<std::pair<glm::vec2, glm::vec2> > > getSlot(const std::string &name);
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