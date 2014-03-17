#include <blib/ParticleSystem.h>

#include <blib/util/FileSystem.h>
#include <blib/ResourceManager.h>
#include <blib/Shader.h>
#include <blib/Util.h>
#include <blib/gl/Vertex.h>
#include <blib/Color.h>
#include <blib/Renderer.h>
#include <blib/Math.h>
#include <blib/util/Log.h>
#include <blib/SpriteBatch.h>
#include <json/json.h>

using blib::util::Log;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace blib
{

	ParticleSystem::ParticleSystem( Renderer* renderer, ResourceManager* resourceManager, SpriteBatch* spriteBatch )
	{
		this->spriteBatch = spriteBatch;
		this->renderer = renderer;
		textureMap = resourceManager->getResource<TextureMap>();
//		renderState = baseRenderState;
		renderState.blendEnabled = true;
		renderState.srcBlendColor = blib::RenderState::SRC_ALPHA;
		renderState.srcBlendAlpha = blib::RenderState::SRC_ALPHA;
		renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;

		shader = resourceManager->getResource<blib::Shader>();
		shader->initFromData(
"precision mediump float;\
attribute vec2 a_position;\
attribute vec4 a_color;\
attribute vec2 a_tex1;\
attribute vec2 a_tex2;\
attribute float a_size;\
varying vec4 color;\
varying vec2 tex1;\
varying vec2 tex2;\
varying float size;\
uniform mat4 matrix;\
uniform mat4 projectionmatrix;\
/*TODO: get the resize factor out of the matrix-matrix and apply it to the pointsize, might be smart to calculate this in software instead of glsl*/\
void main()\
{\
	size = a_size;\
	color = a_color;\
	/*tex1 = a_tex1;\
	tex2 = a_tex2*/;\
	gl_PointSize = a_size;/* * (width / (zoom * 3.0));*/\
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);\
}",			
"precision mediump float;\
varying vec4 color;\
varying float size;\
varying vec2 tex1;\
varying vec2 tex2;\
uniform sampler2D s_texture;\
void main()\
{\
/*	if(size < 0.0)\
		discard;*/\
	/*vec4 col = texture2D(s_texture, tex1 + gl_PointCoord * (tex2-tex1))*/;\
	gl_FragColor = color;/* * col;*/\
}");
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_color", 1);
		shader->bindAttributeLocation("a_tex1", 2);
		shader->bindAttributeLocation("a_tex2", 3);
		shader->bindAttributeLocation("a_size", 4);

		shader->setUniformName(ShaderUniforms::s_texture, "s_texture", Shader::Int);
		shader->setUniformName(ShaderUniforms::projectionmatrix, "projectionmatrix", Shader::Mat4);
		shader->finishUniformSetup();
		shader->setUniform(ShaderUniforms::s_texture, 0);
		renderState.activeShader = shader;
		renderState.activeTexture[0] = textureMap;
		nParticles = 0;
		lastElapsedTime = 0.1f;
	}

	void ParticleSystem::update( double elapsedTime )
	{
		for(std::list<Emitter*>::iterator it = emitters.begin(); it != emitters.end(); it++)
		{
			Emitter* emitter = *it;
			for(int i = (int)glm::floor(emitter->counter * emitter->emitterTemplate->particleCountPerSecondMin); i < (int)glm::floor((emitter->counter + elapsedTime) * emitter->emitterTemplate->particleCountPerSecondMin); i++)
			{
				emitter->newParticle(particles[nParticles], elapsedTime);
				nParticles++;
			}
			emitter->prevPosition = emitter->position;
			emitter->counter += elapsedTime;
			if(emitter->life > 0 && emitter->life != -1)
			{
				emitter->life -= elapsedTime;
				if(emitter->life <= 0)
				{
					//TODO: memory leak! make a list of emitters, and delete it when there's no more particles
					//delete emitter;
					it = emitters.erase(it);
					if(it == emitters.end())
						break;
				}
			}
		}




		int oldParticleCount = nParticles;
		int deadCount = 0;
		for(int i = 0; i < oldParticleCount; i++)
		{
			Particle& p = particles[i];
			if(p.life <= 0)
			{
				deadCount++;
 				nParticles--;
				continue;
			}

			glm::vec2 pos = p.position;

			float friction = (float)glm::pow(1.0 - p.emitter->emitterTemplate->particleProps.friction, elapsedTime);

			p.position = pos + friction * (pos - p.prevPosition) * (float)(lastElapsedTime / elapsedTime)+ p.emitter->emitterTemplate->gravity * (float)elapsedTime;
			p.prevPosition = pos;
			p.life -= (float)(elapsedTime / particles[i].lifeDec);

			p.vertex.position = p.position;

			if(p.life <= 1 && p.life > 0.00001f)
			{
				if(p.emitter->emitterTemplate->particleProps.colors.size() > 1)
				{
					float colorFac = glm::pow(1-p.life, p.emitter->emitterTemplate->particleProps.colorExp) * (p.emitter->emitterTemplate->particleProps.colors.size()-1);
					float factor = colorFac - (int)colorFac;
					p.vertex.color = (1 - factor) * p.emitter->emitterTemplate->particleProps.colors[(int)colorFac] + factor * p.emitter->emitterTemplate->particleProps.colors[(int)colorFac+1];
				}

				if(p.emitter->emitterTemplate->particleProps.size.size() > 1)
				{
					float sizeFac = glm::pow(1-p.life, p.emitter->emitterTemplate->particleProps.sizeExp) * (p.emitter->emitterTemplate->particleProps.size.size()-1);
					float factor = sizeFac - (int)sizeFac;
					p.vertex._size = (1 - factor) * p.emitter->emitterTemplate->particleProps.size[(int)sizeFac] + factor * p.emitter->emitterTemplate->particleProps.size[(int)sizeFac+1];
				}
			}
//			p.vertex.rotation+= (float)(elapsedTime * p.rotationSpeed);

			//maybe use memcpy for this?
			if(deadCount > 0)
				particles[i-deadCount] = p;
		}

		lastElapsedTime = elapsedTime;
	}


	float f = 0.1f;

	void ParticleSystem::draw(glm::mat4 matrix)
	{
		if(nParticles > 0)
		{
/*

			std::vector<VertexP2C4T2T2F1> vertices(nParticles);
			for(int i = 0; i < nParticles; i++)
				vertices.push_back(particles[i].vertex);
			renderState.activeShader->setUniform("matrix", matrix);
			renderer->drawPoints(vertices, renderState);*/

			for(int i = 0; i < nParticles; i++)
			{
				spriteBatch->draw(particles[i].texture, blib::math::easyMatrix(particles[i].vertex.position, 0, 0.01f * particles[i].vertex._size), glm::vec2(32,32), particles[i].vertex.color);
			}

		}
	}

	Emitter* ParticleSystem::addEmitter( std::string name )
	{
		static std::map<std::string, EmitterTemplate*> cache;
		if(cache.find(name) == cache.end())
			cache[name] = new EmitterTemplate(name, textureMap);
		EmitterTemplate* emitterTemplate = cache[name];
		Emitter* emitter = emitterTemplate->getEmitter();
		emitters.push_back(emitter);
		return emitter;
	}


	Emitter* EmitterTemplate::getEmitter()
	{
		return new Emitter(this);
	}


	Emitter::Emitter( EmitterTemplate* emitterTemplate )
	{
		this->emitterTemplate = emitterTemplate;
		position = glm::vec2(0,0);
		prevPosition = position;
		direction = 0;
		counter = 0;
		life = -1;
	}

	void Emitter::newParticle( Particle& particle, double elapsedTime )
	{
		float speed = (float)(blib::math::randomDouble(emitterTemplate->particleProps.speedMin, emitterTemplate->particleProps.speedMax) * elapsedTime);

		particle.life = 1;
		particle.position = position + blib::math::randomFloat() * (position - prevPosition);
		particle.prevPosition = particle.position - speed * blib::util::fromAngle(glm::radians(direction + blib::math::randomFloat(emitterTemplate->particleProps.directionMin, emitterTemplate->particleProps.directionMax)));
		particle.lifeDec = blib::math::randomFloat(emitterTemplate->particleProps.fadeSpeedMin, emitterTemplate->particleProps.fadeSpeedMax);
		particle.texture = emitterTemplate->textureInfos[rand()%emitterTemplate->textureInfos.size()];
		particle.rotationSpeed = glm::radians(blib::math::randomFloat(emitterTemplate->particleProps.rotationMin, emitterTemplate->particleProps.rotationMax));

		particle.vertex.position = position;
		particle.vertex.color = emitterTemplate->particleProps.colors[0];
		particle.vertex.tex1 = particle.texture->t1;
		particle.vertex.tex2 = particle.texture->t2;

		particle.emitter = this;
	}

	void ParticleSystem::clear()
	{
		nParticles = 0;
		for(std::list<Emitter*>::iterator it = emitters.begin(); it != emitters.end(); it++)
			delete *it;	
		emitters.clear();
	}




	template <class T>
	T enumFromString(std::string value, const std::vector<std::pair<T, std::string> > &values)
	{
		for(typename std::vector<std::pair<T, std::string> >::const_iterator it = values.begin(); it != values.end(); it++)
		{
			if(it->second == value)
				return it->first;
		}
		throw "Invalid value";
	}

	EmitterTemplate::EmitterTemplate( std::string filename, TextureMap* textureMap )
	{
		Json::Value data = util::FileSystem::getJson(filename);
		if(data.isNull())
			throw "File not found";


		particleType = enumFromString<ParticleType>(data["particletype"].asString(), util::make_vector<std::pair<ParticleType, std::string> >() << 
			std::pair<ParticleType, std::string>(Fading, "fading") );

		
		if(data["texture"].isString())
			textures.push_back(data["texture"].asString());
		else
			for(Json::ArrayIndex i = 0; i < data["texture"].size(); i++)
				textures.push_back(data["texture"][i].asString());

		for(size_t i = 0; i < textures.size(); i++)
			textureInfos.push_back(textureMap->addTexture("assets/textures/particles/" + textures[i]));




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
		particleCountPerSecondMin = data["particlecount"][0u].asInt();


		particleProps.directionMin = data["particle"]["direction"][0u].asFloat();
		particleProps.directionMax = data["particle"]["direction"][1u].asFloat();

		particleProps.speedMin = data["particle"]["speed"][0u].asFloat();
		particleProps.speedMax = data["particle"]["speed"][1u].asFloat();

		particleProps.rotationMin = data["particle"]["rotation"][0u].asFloat();
		particleProps.rotationMax = data["particle"]["rotation"][1u].asFloat();

		particleProps.friction = data["particle"]["friction"].asFloat();
		particleProps.rotationFriction = data["particle"]["rotationfriction"].asFloat();

		particleProps.fadeSpeedMin = data["particle"]["fadespeed"][0u].asFloat();
		particleProps.fadeSpeedMax = data["particle"]["fadespeed"][1u].asFloat();

	
		particleProps.colorExp = data["particle"]["colorexp"].asFloat();
		for(Json::ArrayIndex i = 0; i < data["particle"]["size"].size(); i++)
			particleProps.size.push_back(data["particle"]["size"][i].asFloat());

		particleProps.sizeExp = data["particle"]["sizeexp"].asFloat();
		for(Json::ArrayIndex i = 0; i < data["particle"]["colors"].size(); i++)
			particleProps.colors.push_back(glm::vec4(data["particle"]["colors"][i][0u].asFloat(), data["particle"]["colors"][i][1u].asFloat(), data["particle"]["colors"][i][2u].asFloat(), data["particle"]["colors"][i][3u].asFloat()));
	}
	void ParticleSystem::resizeGl( int width, int height )
	{
		renderState.activeShader->setUniform(ShaderUniforms::projectionmatrix, glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
	}

}
