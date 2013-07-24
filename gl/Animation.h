#pragma once

#include <string>
#include <list>
#include <map>
#include <blib/gl/SpriteBatch.h>

namespace Json { class Value; }

namespace blib
{
	namespace gl
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

				Bone(Json::Value &config);
				void draw(const Animation& animation, SpriteBatch &spritebatch, glm::mat4 transform);
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
						BoneFrameInfo(Json::Value &config);
					};
					float time;
					std::list<std::pair<Bone*, BoneFrameInfo> > boneInfo;

					KeyFrame(const Animation& animation, Json::Value &config);
				};
				State(const Animation& animation, Json::Value &config);


				float length;
				std::vector<KeyFrame> keyFrames;
			};
			

			double time;
			int currentKeyFrame;
			State* currentState;
			Texture* texture;
			Bone* rootBone;

			std::vector<blib::math::Rectangle> frames;
			std::map<std::string, State*> states;
			std::map<std::string, Bone*> bones;


		public:
			Animation(std::string filename);

			void draw(SpriteBatch &spritebatch, glm::mat4 transform);
			void update(double elapsedTime);
			void setState(std::string newState);
			std::string getState();
		};


	}
}
