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

				};

				std::string name;
				std::vector<KeyFrame> keyFrames;
			};
			

			double time;
			State* currentState;
			Texture* texture;
			Bone* rootBone;

			std::vector<blib::math::Rectangle> frames;
			std::map<std::string, State*> states;

		public:
			Animation(std::string filename);

			void draw(SpriteBatch &spritebatch, glm::mat4 transform);
			void update(double elapsedTime);
			void setState(std::string newState);
			std::string getState();
		};


	}
}
