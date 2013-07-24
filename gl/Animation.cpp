#include <blib/gl/Animation.h>
#include <blib/util/FileSystem.h>
#include <blib/util/Log.h>
using blib::util::Log;

#include <json/value.h>


namespace blib
{
	namespace gl
	{

		Animation::Animation( std::string filename )
		{
			Json::Value config = blib::util::FileSystem::getJson(filename);
			texture = Texture::loadCached("assets/textures/player.png");

			
			for(size_t i = 0; i < config["frames"].size(); i++)
			{
				Json::Value &v = config["frames"][i];
				frames.push_back(blib::math::Rectangle( v["pos"][0].asFloat() / texture->originalWidth, 
														v["pos"][1].asFloat() / texture->originalHeight, 
														v["size"][0].asFloat() / texture->originalWidth, 
														v["size"][1].asFloat() / texture->originalHeight));
			}
			
			Json::Value::Members boneNames = config["base"].getMemberNames();
			for(size_t i = 0; i < boneNames.size(); i++)
				bones[boneNames[i]] = new Bone(config["base"][boneNames[i]]);
			rootBone = bones["root"];


			Json::Value::Members animationNames = config["animations"].getMemberNames();

			for(size_t i = 0; i < animationNames.size(); i++)
			{
				states.insert(std::map<std::string, State*>::value_type(animationNames[i], new State(*this, config["animations"][animationNames[i]])));
			}


			setState(animationNames[0]);
		}


		void Animation::setState(std::string newState)
		{
			time = 0;
			currentState = states[newState];
			currentKeyFrame = 0;
		}

		void Animation::update(double elapsedTime)
		{
			time += elapsedTime;

			int nextKeyFrame = (currentKeyFrame+1)% currentState->keyFrames.size();
			while(time > currentState->keyFrames[nextKeyFrame].time)
			{
				nextKeyFrame++;
				if(nextKeyFrame >= currentState->keyFrames.size())
				{
					nextKeyFrame = 0;
					time -= currentState->length;
				}
			}

			currentKeyFrame = (nextKeyFrame - 1 + currentState->keyFrames.size()) % currentState->keyFrames.size();
		
			for(std::list<std::pair<Bone*, State::KeyFrame::BoneFrameInfo> >::iterator it = currentState->keyFrames[currentKeyFrame].boneInfo.begin(); it != currentState->keyFrames[currentKeyFrame].boneInfo.end(); it++)
			{
				it->first->frame = it->second.frame;
			}


		}

		void Animation::draw(SpriteBatch &spriteBatch, glm::mat4 transform)
		{
			rootBone->draw(*this, spriteBatch, transform);
		}



		/////////////////bone
		Animation::Bone::Bone(Json::Value &config)
		{
			frame = 0;
		}


		void Animation::Bone::draw(const Animation& animation, SpriteBatch &spritebatch, glm::mat4 transform )
		{
			spritebatch.draw(animation.texture, transform, animation.frames[frame].size() * glm::vec2(animation.texture->originalWidth, animation.texture->originalHeight) * 0.5f, animation.frames[frame]);
		}

		/////////////////////////state



		Animation::State::State( const Animation& animation, Json::Value &config )
		{
			length = config["length"].asFloat();
			for(size_t i = 0; i < config["keyframes"].size(); i++)
				keyFrames.push_back(KeyFrame(animation, config["keyframes"][i]));
		}


		///////////////////////keyframe

		Animation::State::KeyFrame::KeyFrame( const Animation& animation, Json::Value &config )
		{
			time = config["time"].asFloat();
			
			Json::Value::Members boneNames = config.getMemberNames();
			for(size_t i = 0; i < boneNames.size(); i++)
			{
				std::map<std::string, Bone*>::const_iterator it = animation.bones.find(boneNames[i]);
				if(it == animation.bones.end())
					continue;
				Bone* bone = it->second;
				
				boneInfo.push_back(std::pair<Bone*, BoneFrameInfo>(bone, BoneFrameInfo(config[boneNames[i]])));

			}
		}

		////////////////////////////boneframeinfo
		Animation::State::KeyFrame::BoneFrameInfo::BoneFrameInfo( Json::Value &config )
		{
			frame = config["frame"].asInt();
		}

	}
}
