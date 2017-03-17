#include <blib/Animation.h>
#include <blib/ResourceManager.h>
#include <blib/util/FileSystem.h>
#include <blib/util/Log.h>
using blib::util::Log;

#include <blib/json.hpp>


namespace blib
{
	Animation::Animation( std::string filename, ResourceManager* resourceManager )
	{
		json config = blib::util::FileSystem::getJson(filename);

		std::string pathName = "";
		if(filename.find("/") != std::string::npos)
			pathName = filename.substr(0, filename.rfind("/")+1);

		texture = resourceManager->getResource<Texture>(pathName + config["texture"].get<std::string>());

			
		for(size_t i = 0; i < config["frames"].size(); i++)
		{
			json &v = config["frames"][i];

			blib::math::Rectangle rect(v["pos"][0].get<float>() / texture->originalWidth,
				v["pos"][1].get<float>() / texture->originalHeight,
				v["size"][0].get<float>() / texture->originalWidth,
				v["size"][1].get<float>() / texture->originalHeight);

			glm::vec2 center;
			if (v.find("center") != v.end())
				center = glm::vec2(	v["center"][0].get<float>() / texture->originalWidth,
									v["center"][1].get<float>() / texture->originalHeight);
			else
				center = rect.center() / glm::vec2(texture->originalWidth, texture->originalHeight);
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
					return;
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
	Animation::Bone::Bone(json &config)
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



	Animation::State::State( const Animation& animation, json &config )
	{
		length = config["length"].get<float>();
		for(size_t i = 0; i < config["keyframes"].size(); i++)
			keyFrames.push_back(KeyFrame(animation, config["keyframes"][i]));
		if(config.find("whendone") != config.end())
			whenDone = config["whendone"].get<std::string>();
	}


	///////////////////////keyframe

	Animation::State::KeyFrame::KeyFrame( const Animation& animation, json &config )
	{
		time = config["time"].get<float>();
			
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
	Animation::State::KeyFrame::BoneFrameInfo::BoneFrameInfo( json &config )
	{
		frame = config["frame"].get<int>();
	}
}
