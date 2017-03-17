#pragma once

#include <string>
#include <list>
#include <map>
#include <blib/SpriteBatch.h>


namespace blib
{


	//TODO: instancing?
	class Animation
	{
		class Bone
		{
		public:
			std::string name;
			std::list<Bone*> children;
			float distanceOrig;
			float angleOrig;
			int frame;

			Bone(json &config);
			void draw(const Animation& animation, SpriteBatch &spritebatch, glm::mat4 transform, const glm::vec4& color = glm::vec4(1,1,1,1));
		};
		class State
		{
		public:
			class KeyFrame
			{
			public:
				class BoneFrameInfo
				{
				public:
					int frame;
					BoneFrameInfo(json &config);
				};
				float time;
				std::list<std::pair<Bone*, BoneFrameInfo> > boneInfo;

				KeyFrame(const Animation& animation, json &config);
			};
			State(const Animation& animation, json &config);

			std::string whenDone;

			float length;
			std::vector<KeyFrame> keyFrames;
		};
			

		double time;
		int currentKeyFrame;
		State* currentState;
		Texture* texture;
		Bone* rootBone;

		std::vector<std::pair<glm::vec2, blib::math::Rectangle> > frames;
		std::map<std::string, State*> states;
		std::map<std::string, Bone*> bones;


	public:
		Animation(std::string filename, ResourceManager* resourceManager);
		~Animation();
		void draw(SpriteBatch &spritebatch, glm::mat4 transform, const glm::vec4& color = glm::vec4(1,1,1,1));
		void update(double elapsedTime);
		void setState(std::string newState);
		std::string getState();
	};
}
