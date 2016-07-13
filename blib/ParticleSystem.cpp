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
#include <blib/json.h>

using blib::util::Log;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace blib
{

	ParticleSystem::ParticleSystem( Renderer* renderer, ResourceManager* resourceManager, SpriteBatch* spriteBatch, int textureSize )
	{
		this->spriteBatch = spriteBatch;
		this->renderer = renderer;
		textureMap = resourceManager->getResource<TextureMap>(textureSize, textureSize);
//		renderState = baseRenderState;
		renderState.blendEnabled = true;
		renderState.srcBlendColor = blib::RenderState::SRC_ALPHA;
		renderState.srcBlendAlpha = blib::RenderState::SRC_ALPHA;
		renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		textureFolder = "assets/textures/particles/";
		shader = resourceManager->getResource<blib::Shader>("ParticleSystem");

		shader->bindAttributeLocation("a_vposition", 0);
		shader->bindAttributeLocation("a_texture", 1);
		shader->bindAttributeLocation("a_color", 2);
		shader->bindAttributeLocation("a_position", 3);
		shader->bindAttributeLocation("a_rotation", 4);
		shader->bindAttributeLocation("a_scale", 5);

		shader->setUniformName(ShaderUniforms::s_texture, "s_texture", Shader::Int);
		shader->setUniformName(ShaderUniforms::projectionmatrix, "projectionmatrix", Shader::Mat4);
		shader->setUniformName(ShaderUniforms::matrix, "matrix", Shader::Mat4);
		shader->finishUniformSetup();
		shader->setUniform(ShaderUniforms::s_texture, 0);
		shader->setUniform(ShaderUniforms::projectionmatrix, glm::ortho(0.0f, (float)1024, (float)768, 0.0f, -1000.0f, 1.0f));
		renderState.activeShader = shader;
		renderState.activeTexture[0] = textureMap;
		renderState.cullFaces = RenderState::CullFaces::NONE;
		nParticlesAdd = 0;
		nParticlesAlpha = 0;
		lastElapsedTime = 0.1f;

		particleData = new VertexDef[MAX_PARTICLES * 8];
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			addParticles[i].vertex = particleData + (4 * i);
			alphaParticles[i].vertex = particleData + 4 * (MAX_PARTICLES + i);
		}

		vbo = resourceManager->getResource<blib::VBO>();
		vio = resourceManager->getResource<blib::VIO>();
		vbo->setVertexFormat<VertexDef>();
		vio->setElementType<unsigned int>();

		std::vector<unsigned int> indices;
		for (int i = 0; i < MAX_PARTICLES*2; i++)
		{
			indices.push_back(4 * i + 0);
			indices.push_back(4 * i + 1);
			indices.push_back(4 * i + 2);

			indices.push_back(4 * i + 1);
			indices.push_back(4 * i + 3);
			indices.push_back(4 * i + 2);
		}
		renderer->setVio(vio, indices);
		renderer->setVbo<VertexDef>(vbo, NULL, MAX_PARTICLES*4*2);
		renderState.activeVbo = vbo;
		renderState.activeVio = vio;

	}

	void ParticleSystem::update( double elapsedTime )
	{
	//	if (elapsedTime > 3 * lastElapsedTime)
	//		elapsedTime = lastElapsedTime;
		for(std::list<Emitter*>::iterator it = emitters.begin(); it != emitters.end(); it++)
		{
			Emitter* emitter = *it;
			if (!emitter->enabled)
			{
				emitter->prevPosition = emitter->position;
				emitter->counter += elapsedTime;
				continue;
			}
			for(int i = (int)glm::floor(emitter->counter * emitter->emitterTemplate->particleCountPerSecondMin); i < (int)glm::floor((emitter->counter + elapsedTime) * emitter->emitterTemplate->particleCountPerSecondMin); i++)
			{
                if(nParticlesAdd < MAX_PARTICLES-1)
				if (emitter->emitterTemplate->blendMode == EmitterTemplate::Add)
					emitter->newParticle(addParticles[nParticlesAdd++], elapsedTime);
                if(nParticlesAlpha < MAX_PARTICLES-1)
				if (emitter->emitterTemplate->blendMode == EmitterTemplate::Alpha)
					emitter->newParticle(alphaParticles[nParticlesAlpha++], elapsedTime);
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


		updateParticles(addParticles, nParticlesAdd, elapsedTime);
		updateParticles(alphaParticles, nParticlesAlpha, elapsedTime);



		lastElapsedTime = elapsedTime;
	}


	void ParticleSystem::updateParticles(Particle* particles, int& nParticles, double elapsedTime)
	{
		int oldParticleCount = nParticles;
		int deadCount = 0;
		for (int i = 0; i < oldParticleCount; i++)
		{
			Particle& p = particles[i];
			if (p.life <= 0)
			{
				deadCount++;
				nParticles--;
				continue;
			}

			glm::vec2 pos = p.position;

			float friction = (float)glm::pow(1.0 - p.emitter->emitterTemplate->particleProps.friction, elapsedTime);

			p.position = pos + friction * (pos - p.prevPosition)/* * (float)(lastElapsedTime / elapsedTime)*/ + p.emitter->emitterTemplate->gravity * (float)elapsedTime;
			p.prevPosition = pos;
			p.life -= (float)(elapsedTime / particles[i].lifeDec);

			for (int i = 0; i < 4; i++)
				p.vertex[i].position = p.position;

			if (p.life <= 1 && p.life > 0.00001f)
			{
				if (p.emitter->emitterTemplate->particleProps.colors.size() > 1)
				{
					float colorFac = glm::pow(1 - p.life, p.emitter->emitterTemplate->particleProps.colorExp) * (p.emitter->emitterTemplate->particleProps.colors.size() - 1);
					float factor = colorFac - (int)colorFac;
					for (int ii = 0; ii < 4; ii++)
						p.vertex[ii].color = (1 - factor) * p.emitter->emitterTemplate->particleProps.colors[(int)colorFac] + factor * p.emitter->emitterTemplate->particleProps.colors[(int)colorFac + 1];
				}

				if (p.emitter->emitterTemplate->particleProps.size.size() > 1)
				{
					float sizeFac = glm::pow(1 - p.life, p.emitter->emitterTemplate->particleProps.sizeExp) * (p.emitter->emitterTemplate->particleProps.size.size() - 1);
					float factor = sizeFac - (int)sizeFac;
					for (int ii = 0; ii < 4; ii++)
						p.vertex[ii].scale = ((1 - factor) * p.emitter->emitterTemplate->particleProps.size[(int)sizeFac] + factor * p.emitter->emitterTemplate->particleProps.size[(int)sizeFac + 1]) / 4.0f;
				}
			}
			for (int ii = 0; ii < 4; ii++)
				p.vertex[ii].rotation += (float)(elapsedTime * glm::radians(p.rotationSpeed));

			//maybe use memcpy for this?
			if (deadCount > 0)
			{
				VertexDef* old = particles[i - deadCount].vertex;
				VertexDef tmp[4];
				memcpy(tmp, old, sizeof(VertexDef) * 4);
				memcpy(old, p.vertex, sizeof(VertexDef) * 4);
				memcpy(p.vertex, tmp, sizeof(VertexDef) * 4);
	
				particles[i - deadCount].vertex = p.vertex;
//				p.vertex = old;

				particles[i - deadCount] = p;
				particles[i - deadCount].vertex = old;
			}
		}
	}





	float f = 0.1f;

	void ParticleSystem::draw(glm::mat4 matrix)
	{
		/*for (int i = 0; i < nParticlesAlpha; i++)
			spriteBatch->draw(alphaParticles[i].texture, blib::math::easyMatrix(alphaParticles[i].vertex.position, alphaParticles[i].rotation, 0.01f * alphaParticles[i].vertex._size), alphaParticles[i].texture->center, alphaParticles[i].vertex.color);
		spriteBatch->end();
		spriteBatch->renderState.dstBlendColor = blib::RenderState::ONE;
		spriteBatch->renderState.dstBlendAlpha = blib::RenderState::ONE;
		spriteBatch->begin(spriteBatch->getMatrix());

		for (int i = 0; i < nParticlesAdd; i++)
			spriteBatch->draw(addParticles[i].texture, blib::math::easyMatrix(addParticles[i].vertex.position, addParticles[i].rotation, 0.01f * addParticles[i].vertex._size), addParticles[i].texture->center, addParticles[i].vertex.color);

		spriteBatch->end();
		spriteBatch->renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		spriteBatch->renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;
		spriteBatch->begin(spriteBatch->getMatrix());*/


		renderState.activeShader->setUniform(ShaderUniforms::matrix, matrix);

		if (nParticlesAdd > 0)
		{
			renderer->setVboSub(vbo, 0, particleData, nParticlesAdd * 4);

			renderState.dstBlendColor = blib::RenderState::ONE;
			renderState.dstBlendAlpha = blib::RenderState::ONE;
			renderer->drawIndexedTriangles<VertexDef>(0, nParticlesAdd*6, renderState);
		}
		if (nParticlesAlpha > 0)
		{
			renderer->setVboSub(vbo, MAX_PARTICLES * 4, particleData + MAX_PARTICLES * 4, nParticlesAlpha * 4);
			renderState.dstBlendColor = blib::RenderState::ONE_MINUS_SRC_ALPHA;
			renderState.dstBlendAlpha = blib::RenderState::ONE_MINUS_SRC_ALPHA;
			renderer->drawIndexedTriangles<VertexDef>(MAX_PARTICLES*6, nParticlesAlpha * 6, renderState);
		}

	}

	Emitter* ParticleSystem::addEmitter( std::string name )
	{
		if(cache.find(name) == cache.end())
			cache[name] = new EmitterTemplate(name, textureMap, textureFolder);
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
		enabled = true;
		lastTextureSelected = 0;
	}

	void Emitter::newParticle( Particle& particle, double elapsedTime )
	{
		float speed = (float)(blib::math::randomDouble(emitterTemplate->particleProps.speedMin, emitterTemplate->particleProps.speedMax) * elapsedTime);

		particle.life = 1;
		particle.position = position + blib::math::randomFloat() * (position - prevPosition) +
			glm::vec2(blib::math::randomFloat(emitterTemplate->initialSpreadX.x, emitterTemplate->initialSpreadX.y), blib::math::randomFloat(emitterTemplate->initialSpreadY.x, emitterTemplate->initialSpreadY.y)) * blib::math::fromAngle(blib::math::randomFloat(0, blib::math::pif));
		particle.prevPosition = particle.position -speed * blib::util::fromAngle(glm::radians(direction + blib::math::randomFloat(emitterTemplate->particleProps.directionMin, emitterTemplate->particleProps.directionMax)));
		particle.lifeDec = blib::math::randomFloat(emitterTemplate->particleProps.fadeSpeedMin, emitterTemplate->particleProps.fadeSpeedMax);
		if(emitterTemplate->textureOrder == EmitterTemplate::Random)
			particle.texture = emitterTemplate->textureInfos[rand()%emitterTemplate->textureInfos.size()];
		else
		{
			lastTextureSelected = (lastTextureSelected + 1) % emitterTemplate->textureInfos.size();
			particle.texture = emitterTemplate->textureInfos[lastTextureSelected];
		}
		particle.rotationSpeed = blib::math::randomFloat(emitterTemplate->particleProps.rotationSpeedMin, emitterTemplate->particleProps.rotationSpeedMax);
		particle.rotation = blib::math::randomFloat(emitterTemplate->particleProps.rotationMin, emitterTemplate->particleProps.rotationMax) + direction;


		static glm::vec2 offsets[4] = { glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(-1, 1), glm::vec2(1, 1) };

		for (int i = 0; i < 4; i++)
		{
			particle.vertex[i].position = position;
			particle.vertex[i].color = emitterTemplate->particleProps.colors[0];
			particle.vertex[i].texCoord = particle.texture->t1 + ((offsets[i] + glm::vec2(1, 1)) / 2.0f) * (particle.texture->t2 - particle.texture->t1);
			particle.vertex[i].position2 = offsets[i] * glm::vec2(particle.texture->width, particle.texture->width) / 64.0f;
			particle.vertex[i].rotation = glm::radians(particle.rotation);
		}

		particle.emitter = this;
	}

	void ParticleSystem::clear()
	{
		nParticlesAdd = 0;
		nParticlesAlpha = 0;
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

	EmitterTemplate::EmitterTemplate( std::string filename, TextureMap* textureMap, const std::string &textureFolder)
	{
		json::Value data = util::FileSystem::getJson(filename);
		if(data.isNull())
			throw "File not found";


		particleType = enumFromString<ParticleType>(data["particletype"], util::make_vector<std::pair<ParticleType, std::string> >() << 
			std::pair<ParticleType, std::string>(Fading, "fading") );

		
		if(data["texture"].isString())
			textures.push_back(data["texture"].asString());
		else
			for(size_t i = 0; i < data["texture"].size(); i++)
				textures.push_back(data["texture"][i].asString());

		for(size_t i = 0; i < textures.size(); i++)
			textureInfos.push_back(textureMap->addTexture(textureFolder + textures[i]));




		textureOrder = enumFromString<TextureOrder>(data["textureorder"].asString(), util::make_vector<std::pair<TextureOrder, std::string> >() << 
			std::pair<TextureOrder, std::string>(Random, "random")<<
			std::pair<TextureOrder, std::string>(Ordered, "order"));


		blendMode = enumFromString<BlendMode>(data["blendmode"].asString(), util::make_vector<std::pair<BlendMode, std::string> >() << 
			std::pair<BlendMode, std::string>(Alpha, "alpha")<<
			std::pair<BlendMode, std::string>(Add, "add") );

		shape = enumFromString<Shape>(data["shape"].asString(), util::make_vector<std::pair<Shape, std::string> >() << 
			std::pair<Shape, std::string>(Point, "point")<<
			std::pair<Shape, std::string>(Line, "line")<<
			std::pair<Shape, std::string>(Circle, "circle") );

		gravity = glm::vec2(data["gravity"][0].asFloat(), data["gravity"][1].asFloat());
		collision = data["collision"].asBool();
		particleCountPerSecondMin = data["particlecount"][0].asInt();


		particleProps.directionMin = data["particle"]["direction"][0].asFloat();
		particleProps.directionMax = data["particle"]["direction"][1].asFloat();

		particleProps.speedMin = data["particle"]["speed"][0].asFloat();
		particleProps.speedMax = data["particle"]["speed"][1].asFloat();

		particleProps.rotationMin = data["particle"]["rotation"][0].asFloat();
		particleProps.rotationMax = data["particle"]["rotation"][1].asFloat();

		particleProps.rotationSpeedMin = data["particle"]["rotationspeed"][0].asFloat();
		particleProps.rotationSpeedMax = data["particle"]["rotationspeed"][1].asFloat();

		particleProps.friction = data["particle"]["friction"].asFloat();
		particleProps.rotationFriction = data["particle"]["rotationfriction"].asFloat();

		particleProps.fadeSpeedMin = data["particle"]["fadespeed"][0].asFloat();
		particleProps.fadeSpeedMax = data["particle"]["fadespeed"][1].asFloat();

	
		particleProps.colorExp = data["particle"]["colorexp"].asFloat();
		for(size_t i = 0; i < data["particle"]["size"].size(); i++)
			particleProps.size.push_back(data["particle"]["size"][i].asFloat());

		particleProps.sizeExp = data["particle"]["sizeexp"].asFloat();
		for(size_t i = 0; i < data["particle"]["colors"].size(); i++)
			particleProps.colors.push_back(glm::vec4(data["particle"]["colors"][i][0].asFloat(), data["particle"]["colors"][i][1].asFloat(), data["particle"]["colors"][i][2].asFloat(), data["particle"]["colors"][i][3].asFloat()));

		initialSpreadX = glm::vec2(0, 0);
		initialSpreadY = glm::vec2(0, 0);
		if (data.isMember("initialspread"))
		{
			if (data["initialspread"][0].isArray())
			{
				initialSpreadX = glm::vec2(data["initialspread"][0][0], data["initialspread"][0][1]);
				initialSpreadY = glm::vec2(data["initialspread"][1][0], data["initialspread"][1][1]);
			}
			else
			{
				initialSpreadX = glm::vec2(data["initialspread"][0], data["initialspread"][1]);
				initialSpreadY = glm::vec2(data["initialspread"][0], data["initialspread"][1]);
			}
		}

	}
	void ParticleSystem::resizeGl( int width, int height )
	{
		renderState.activeShader->setUniform(ShaderUniforms::projectionmatrix, glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
	}

	ParticleSystem::~ParticleSystem()
	{
		blib::ResourceManager::getInstance().dispose(textureMap);
		//blib::ResourceManager::getInstance().dispose(shader);
	}

	void ParticleSystem::preCache(std::string dir)
	{
		if (dir[dir.length() - 1] != '/')
			dir += "/";
		std::vector<std::string> files = blib::util::FileSystem::getFileList(dir);
		Log::out << "Caching ";
		for (const std::string &file : files)
		{
			if (file[0] == '.')
				continue;
			Log::out << file << " ";
			if (cache.find(dir + file) == cache.end())
				cache[dir + file] = new EmitterTemplate(dir + file, textureMap, textureFolder);
		}
		Log::out << Log::newline;
	}

}
