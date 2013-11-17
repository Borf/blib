#include <blib/ParticleSystem.h>

#include <blib/util/FileSystem.h>
#include <blib/ResourceManager.h>
#include <blib/Shader.h>
#include <blib/Util.h>
#include <blib/gl/Vertex.h>
#include <blib/Color.h>
#include <blib/Renderer.h>
#include <blib/Math.h>
#include <json/json.h>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace blib
{

	ParticleSystem::ParticleSystem( Renderer* renderer, ResourceManager* resourceManager )
	{
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
attribute float a_rotation;\
varying vec4 color;\
varying vec2 tex1;\
varying vec2 tex2;\
varying float size;\
varying mat2 rotation;\
uniform mat4 matrix;\
uniform mat4 projectionmatrix;\
void main()\
{\
	size = a_size;\
	color = a_color;\
	tex1 = a_tex1;\
	tex2 = a_tex2;\
	rotation = mat2( cos( a_rotation ), -sin( a_rotation ), \
					 sin( a_rotation ),  cos( a_rotation ));\
	gl_PointSize = a_size;/* * (width / (zoom * 3.0));*/\
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);\
}",			
"precision mediump float;\
varying vec4 color;\
varying float size;\
varying vec2 tex1;\
varying vec2 tex2;\
varying mat2 rotation;\
uniform sampler2D s_texture;\
void main()\
{\
	if(size < 0.0)\
		discard;\
	vec2 pos = gl_PointCoord - vec2(0.5, 0.5);\
	pos = pos * rotation;\
	pos = pos + vec2(0.5, 0.5);\
	vec4 col = texture2D(s_texture, tex1 + pos * (tex2-tex1));\
	gl_FragColor = color * col;\
}");
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_color", 1);
		shader->bindAttributeLocation("a_tex1", 2);
		shader->bindAttributeLocation("a_tex2", 3);
		shader->bindAttributeLocation("a_rotation", 4);
		shader->bindAttributeLocation("a_size", 5);
		shader->setUniform("s_texture", 0);
		renderState.activeShader = shader;
		renderState.activeTexture[0] = textureMap;
		nParticles = 0;
		lastElapsedTime = 0.1f;
	}

	void ParticleSystem::update( double elapsedTime )
	{
		//if(rand() % 2 == 0)
		{
			Emitter* emitter = *emitters.begin();
			emitter->newParticle(particles[nParticles]);
			nParticles++;
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
			p.position = pos + (pos - p.prevPosition) * (float)(elapsedTime / lastElapsedTime) + p.emitter->emitterTemplate->gravity * (float)elapsedTime;
			p.prevPosition = pos;
			p.life -= (float)(elapsedTime / particles[i].lifeDec);

			p.vertex.position = p.position;
			p.vertex.color = Color::white;
			p.vertex.color.a = 1-glm::pow(1-p.life, 0.5f);
			p.vertex._size = 10 + 90 * glm::pow(1-p.life, 1.5f);
			p.vertex.rotation+=elapsedTime * p.rotationSpeed;

			//maybe use memcpy for this?
			if(deadCount > 0)
				particles[i-deadCount] = p;
		}

		lastElapsedTime = elapsedTime;
	}


	float f = 0.1f;

	void ParticleSystem::draw()
	{
		if(nParticles > 0)
		{
			std::vector<VertexP2C4T2T2F1F1> vertices(nParticles);
			for(int i = 0; i < nParticles; i++)
				vertices.push_back(particles[i].vertex);
			renderState.activeShader->setUniform("matrix", glm::mat4());
			renderer->drawPoints(vertices, renderState);
		}
	}

	Emitter* ParticleSystem::addEmitter( std::string name )
	{
		EmitterTemplate* emitterTemplate = new EmitterTemplate(name, textureMap);
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
		direction = 0;
	}

	void Emitter::newParticle( Particle& particle )
	{
		float speed = blib::math::randomFloat(emitterTemplate->particleProps.speedMin, emitterTemplate->particleProps.speedMax);

		particle.life = 1;
		particle.position = position;
		particle.prevPosition = position - speed * blib::util::fromAngle(glm::radians(direction + blib::math::randomFloat(emitterTemplate->particleProps.directionMin, emitterTemplate->particleProps.directionMax)));
		particle.lifeDec = blib::math::randomFloat(emitterTemplate->particleProps.fadeSpeedMin, emitterTemplate->particleProps.fadeSpeedMax);
		particle.texture = emitterTemplate->textureInfos[rand()%emitterTemplate->textureInfos.size()];
		particle.rotationSpeed = glm::radians(blib::math::randomFloat(emitterTemplate->particleProps.rotationMin, emitterTemplate->particleProps.rotationMax));

		particle.vertex.tex1 = particle.texture->t1;
		particle.vertex.tex2 = particle.texture->t2;
		particle.vertex.rotation = 0;

		particle.emitter = this;
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
			for(size_t i = 0; i < data["texture"].size(); i++)
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

		for(size_t i = 0; i < data["particle"]["size"].size(); i++)
			particleProps.size.push_back(data["particle"]["size"][i].asFloat());

		for(size_t i = 0; i < data["particle"]["colors"].size(); i++)
			particleProps.colors.push_back(glm::vec4(data["particle"]["colors"][i][0u].asFloat(), data["particle"]["colors"][i][1u].asFloat(), data["particle"]["colors"][i][2u].asFloat(), data["particle"]["colors"][i][3u].asFloat()));
	}
	void ParticleSystem::resizeGl( int width, int height )
	{
		renderState.activeShader->setUniform("projectionmatrix", glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1.0f));
	}

}