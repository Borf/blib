#include <blib/gl/Animation.h>
#include <blib/util/FileSystem.h>
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


			rootBone = new Bone(config["base"]["root"]);
			

		}


		void Animation::setState(std::string newState)
		{
			time = 0;
			currentState = states[newState];
		}

		void Animation::update(double elapsedTime)
		{
			time += elapsedTime;
		}

		void Animation::draw(SpriteBatch &spriteBatch, glm::mat4 transform)
		{
			rootBone->draw(*this, spriteBatch, transform);
		}




		Animation::Bone::Bone(Json::Value &config)
		{
			frame = 0;
		}


		void Animation::Bone::draw(const Animation& animation, SpriteBatch &spritebatch, glm::mat4 transform )
		{
			spritebatch.draw(animation.texture, transform, animation.frames[frame].size() * glm::vec2(animation.texture->originalWidth, animation.texture->originalHeight) * 0.5f, animation.frames[frame]);
		}

	}
}
