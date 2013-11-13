#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <blib/RenderState.h>
#include <blib/gl/GlResizeRegister.h>

namespace blib
{
	class Renderer;
	class ResourceManager;
	class EmitterTemplate;
	class Shader;


	struct ParticleData
	{
		glm::vec2 position;
		glm::vec4 color;
		float rotation;
		float size;
	};

	class ParticleBase
	{
	public:
		glm::vec2 position;
		glm::vec2 prevPosition;
		float life;


		ParticleBase()
		{
			life = 1;
		}
	};

	class Emitter
	{
		EmitterTemplate* emitterTemplate;
	public:
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
			std::vector<glm::vec4> colors;
		} particleProps;
		Emitter* getEmitter();
	};

	class ParticleSystem : public gl::GlResizeRegister
	{
		Renderer* renderer;
		Shader* shader;
		RenderState renderState;
	public:
		ParticleSystem(Renderer* renderer, ResourceManager* resourceManager);
		void update(double elapsedTime);
		void draw();


		void clear();
		Emitter* addEmitter(std::string name);
		void removeEmitter(Emitter* emitter);

		Attractor* addAttractor();
		void removeAttractor();


		void resizeGl( int width, int height );
	};


}