#include <blib/Animation.h>
#include <blib/ResourceManager.h>
#include <blib/util/FileSystem.h>
#include <blib/util/Log.h>
using blib::util::Log;

#include <blib/json.h>


namespace blib
{
	Animation::Animation( std::string filename, ResourceManager* resourceManager )
	{
		blib::json::Value config = blib::util::FileSystem::getJson(filename);

		std::string pathName = "";
		if(filename.find("/") != std::string::npos)
			pathName = filename.substr(0, filename.rfind("/")+1);

		texture = resourceManager->getResource<Texture>(pathName + config["texture"].asString());

			
		for(size_t i = 0; i < config["frames"].size(); i++)
		{
			blib::json::Value &v = config["frames"][i];

			blib::math::Rectangle rect(v["pos"][0].asFloat() / texture->originalWidth,
				v["pos"][1].asFloat() / texture->originalHeight,
				v["size"][0].asFloat() / texture->originalWidth,
				v["size"][1].asFloat() / texture->originalHeight);

			glm::vec2 center;
			if (v.isMember("center"))
				center = glm::vec2(	v["center"][0].asFloat() / texture->originalWidth,
									v["center"][1].asFloat() / texture->originalHeight);
			else
				center = rect.center();
			frames.push_back(std::pair<glm::vec2, blib::math::Rectangle>(center, rect));
		}
			
		
		for (auto it = config["base"].begin(); it != config["base"].end(); it++)
			bones[it.key()] = new Bone(it.value());
		rootBone = bones["root"];



		for (auto it = config["animations"].begin(); it != config["animations"].end(); it++)
		{
			states.insert(std::map<std::string, State*>::value_type(it.key(), new State(*this, it.value())));
		}


		setState(config["animations"].begin().key());
	}

	Animation::~Animation()
	{
		blib::ResourceManager::getInstance().dispose(texture);
	}


	void Animation::setState(std::string newState)
	{
		if (currentState == states[newState])
			return;
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
			if(nextKeyFrame >= (int)currentState->keyFrames.size())
			{
				if (currentState->whenDone != "")
				{
					setState(currentState->whenDone);
				}
				else
				{
					nextKeyFrame = 0;
					time -= currentState->length;
				}
			}	
		}

		currentKeyFrame = (nextKeyFrame - 1 + currentState->keyFrames.size()) % currentState->keyFrames.size();
		
		for(std::list<std::pair<Bone*, State::KeyFrame::BoneFrameInfo> >::iterator it = currentState->keyFrames[currentKeyFrame].boneInfo.begin(); it != currentState->keyFrames[currentKeyFrame].boneInfo.end(); it++)
		{
			it->first->frame = it->second.frame;
		}


	}

	void Animation::draw(SpriteBatch &spriteBatch, glm::mat4 transform, const glm::vec4& color)
	{
		rootBone->draw(*this, spriteBatch, transform, color);
	}



	/////////////////bone
	Animation::Bone::Bone(json::Value &config)
	{
		frame = 0;
	}


	void Animation::Bone::draw(const Animation& animation, SpriteBatch &spritebatch, glm::mat4 transform, const glm::vec4& color)
	{
		spritebatch.draw(animation.texture, transform, 
			animation.frames[frame].first * glm::vec2(animation.texture->originalWidth, animation.texture->originalHeight), 
			animation.frames[frame].second, color);
	}

	/////////////////////////state



	Animation::State::State( const Animation& animation, json::Value &config )
	{
		length = config["length"].asFloat();
		for(size_t i = 0; i < config["keyframes"].size(); i++)
			keyFrames.push_back(KeyFrame(animation, config["keyframes"][i]));
		if(config.isMember("whendone"))
			whenDone = config["whendone"].asString();
	}


	///////////////////////keyframe

	Animation::State::KeyFrame::KeyFrame( const Animation& animation, json::Value &config )
	{
		time = config["time"].asFloat();
			
		for (auto it2 = config.begin(); it2 != config.end(); it2++)
		{
			std::map<std::string, Bone*>::const_iterator it = animation.bones.find(it2.key());
			if(it == animation.bones.end())
				continue;
			Bone* bone = it->second;
				
			boneInfo.push_back(std::pair<Bone*, BoneFrameInfo>(bone, BoneFrameInfo(it2.value())));

		}
	}

	////////////////////////////boneframeinfo
	Animation::State::KeyFrame::BoneFrameInfo::BoneFrameInfo( json::Value &config )
	{
		frame = config["frame"].asInt();
	}
}
