#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <blib/RenderState.h>
#include <blib/TextureMap.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Vertex.h>

#define MAX_PARTICLES 1000000

namespace blib
{
	class Renderer;
	class ResourceManager;
	class EmitterTemplate;
	class Emitter;
	class Shader;
	class TextureMap;


	class Particle
	{
	public:
		glm::vec2 position;
		glm::vec2 prevPosition;
		float life;
		float lifeDec;
		TextureMap::TexInfo* texture;
		float rotationSpeed;

		VertexP2C4T2T2F1F1 vertex;
		Particle()
		{
			life = 1;
		}
		Emitter* emitter;
	};

	class Emitter
	{
	public:
		EmitterTemplate* emitterTemplate;
		glm::vec2 position;
		glm::vec2 prevPosition;
		float direction;

		double counter;

		Emitter(EmitterTemplate* emitterTemplate);
		void newParticle(Particle& particle, double elapsedTime);
	};

	class Attractor
	{

	};

	class EmitterTemplate
	{
	public:
		EmitterTemplate(std::string filename, TextureMap* textureMap);

		enum ParticleType
		{
				Fading
		} particleType;
		std::vector<std::string> textures;
		std::vector<TextureMap::TexInfo*> textureInfos;

		enum TextureOrder
		{
			Random,
		} textureOrder;

		enum BlendMode
		{
			Alpha,
			Add
		} blendMode;

		enum Shape
		{
			Point,
			Line,
			Circle
		} shape;

		glm::vec2 gravity;
		bool collision;
		int particleCountPerSecondMin;
		int particleCountPerSecondMax;


		struct ParticleProps
		{
			float directionMin;
			float directionMax; 

			float speedMin;
			float speedMax;

			float rotationMin;
			float rotationMax;

			float friction;
			float rotationFriction;
			float fadeSpeedMin;
			float fadeSpeedMax;

			std::vector<float> size;
			float sizeExp;
			std::vector<glm::vec4> colors;
			float colorExp;
		} particleProps;
		Emitter* getEmitter();
	};

	class ParticleSystem : public gl::GlResizeRegister
	{
		double lastElapsedTime;
		Renderer* renderer;
		Shader* shader;
		RenderState renderState;
		std::list<Emitter*> emitters;
		TextureMap* textureMap;
	public:
		ParticleSystem(Renderer* renderer, ResourceManager* resourceManager);
		void update(double elapsedTime);
		void draw(glm::mat4 matrix);


		Particle particles[MAX_PARTICLES];
		int nParticles;

		void clear();
		Emitter* addEmitter(std::string name);
		void removeEmitter(Emitter* emitter);

		Attractor* addAttractor();
		void removeAttractor();


		void resizeGl( int width, int height );
	};


}