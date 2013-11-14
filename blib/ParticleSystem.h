#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <blib/RenderState.h>
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


	class Particle
	{
	public:
		glm::vec2 position;
		glm::vec2 prevPosition;
		float life;

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
		float direction;


		Emitter(EmitterTemplate* emitterTemplate);
	};

	class Attractor
	{

	};

	class EmitterTemplate
	{
	public:
		EmitterTemplate(std::string filename);

		enum ParticleType
		{
				Fading
		} particleType;
		std::vector<std::string> textures;

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
			float fadeSpeedExp;

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
//		TextureMap* textureMap;
	public:
		ParticleSystem(Renderer* renderer, ResourceManager* resourceManager);
		void update(double elapsedTime);
		void draw();


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