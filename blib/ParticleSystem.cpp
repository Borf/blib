#include <blib/ParticleSystem.h>

#include <blib/util/FileSystem.h>
#include <blib/Util.h>
#include <json/json.h>
#include <tuple>

namespace blib
{

	ParticleSystem::ParticleSystem( Renderer* renderer )
	{
		this->renderer = renderer;
	}

	void ParticleSystem::update( double elapsedTime )
	{

	}

	void ParticleSystem::draw()
	{

	}

	Emitter* ParticleSystem::addEmitter( std::string name )
	{
		EmitterTemplate* emitterTemplate = new EmitterTemplate(name);
		return emitterTemplate->getEmitter();
	}


	Emitter* EmitterTemplate::getEmitter()
	{
		return new Emitter(this);
	}


	Emitter::Emitter( EmitterTemplate* emitterTemplate )
	{
		this->emitterTemplate = emitterTemplate;
	}




	template <class T>
	T enumFromString(std::string value, const std::vector<std::pair<T, std::string> > &values)
	{
		for(std::vector<std::pair<T, std::string> >::const_iterator it = values.begin(); it != values.end(); it++)
		{
			if(it->second == value)
				return it->first;
		}
		throw "Invalid value";
	}

	EmitterTemplate::EmitterTemplate( std::string filename )
	{
		Json::Value data = util::FileSystem::getJson(filename);
		if(data.isNull())
			throw "File not found";


		particleType = enumFromString<ParticleType>(data["particletype"].asString(), util::make_vector<std::pair<ParticleType, std::string> >() << 
			std::pair<ParticleType, std::string>(Fading, "fading") );

		
		if(data["texture"].isString())
			textures.push_back(data["texture"].asString());
		else
			for(size_t i = 0; i < data["texture"].size(); i++)
				textures.push_back(data["texture"][i].asString());


		textureOrder = enumFromString<TextureOrder>(data["textureorder"].asString(), util::make_vector<std::pair<TextureOrder, std::string> >() << 
			std::pair<TextureOrder, std::string>(Random, "random") );


		blendMode = enumFromString<BlendMode>(data["blendmode"].asString(), util::make_vector<std::pair<BlendMode, std::string> >() << 
			std::pair<BlendMode, std::string>(Alpha, "alpha")<<
			std::pair<BlendMode, std::string>(Add, "add") );

		shape = enumFromString<Shape>(data["shape"].asString(), util::make_vector<std::pair<Shape, std::string> >() << 
			std::pair<Shape, std::string>(Point, "point")<<
			std::pair<Shape, std::string>(Line, "line")<<
			std::pair<Shape, std::string>(Circle, "circle") );

		gravity = glm::vec2(data["gravity"][0u].asFloat(), data["gravity"][1u].asFloat());
		collision = data["collision"].asBool();




	}

}