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
attribute float a_size;\
attribute float a_rotation;\
varying vec4 color;\
varying float size;\
uniform mat4 matrix;\
uniform mat4 projectionmatrix;\
void main()\
{\
	size = a_size;\
	color = a_color;\
	gl_PointSize = 2;/*a_size * (width / (zoom * 3.0));*/\
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);\
}",			
"precision mediump float;\
varying vec4 color;\
varying float size;\
uniform sampler2D s_texture;\
void main()\
{\
	if(size < 0.0)\
		discard;\
	vec4 col = texture2D(s_texture, gl_PointCoord);\
	gl_FragColor = color;\
}");
		shader->bindAttributeLocation("a_position", 0);
		shader->bindAttributeLocation("a_texture", 1);
		shader->bindAttributeLocation("a_rotation", 2);
		shader->bindAttributeLocation("a_size", 3);
		shader->setUniform("s_texture", 0);
		shader->setUniform("s_texture", 0);
		renderState.activeShader = shader;

		nParticles = 0;

	}

	void ParticleSystem::update( double elapsedTime )
	{
		if(rand() % 3 == 0)
		{
			particles[nParticles].life = 1;
			particles[nParticles].position = glm::vec2(1280, 720) / 2.0f;
			particles[nParticles].prevPosition = particles[nParticles].position + blib::math::randomFloat(0.1f) * blib::util::fromAngle(blib::math::randomFloat(2*M_PI));
			particles[nParticles].emitter = NULL;
			nParticles++;
		}

		int oldParticleCount = nParticles;
		int deadCount = 0;
		for(int i = 0; i < oldParticleCount; i++)
		{
			if(particles[i].life <= 0)
			{
				deadCount++;
 				nParticles--;
				continue;
			}


			glm::vec2 pos = particles[i].position;
			particles[i].position = pos + 0.9998f * (pos - particles[i].prevPosition) + glm::vec2(0,0.0001f);
			particles[i].prevPosition = pos;
			particles[i].life -= 0.001f * blib::math::randomFloat();



			particles[i].vertex.position = particles[i].position;
			particles[i].vertex.color = Color::white;
			particles[i].vertex.color.a = particles[i].life;
			particles[i].vertex._size = 10;

			//maybe use memcpy for this?
			if(deadCount > 0)
				particles[i-deadCount] = particles[i];
		}

		if(nParticles %100 == 0)
			printf("Particles: %i\n", nParticles);
	}


	float f = 0.1f;

	void ParticleSystem::draw()
	{
		if(nParticles > 0)
		{
			std::vector<VertexP2C4F1F1> vertices(nParticles);
			for(size_t i = 0; i < nParticles; i++)
				vertices.push_back(particles[i].vertex);
			renderState.activeShader->setUniform("matrix", glm::mat4());
			renderer->drawPoints(vertices, renderState);
		}
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