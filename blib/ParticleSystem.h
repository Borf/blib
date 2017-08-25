#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <blib/RenderState.h>
#include <blib/TextureMap.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Vertex.h>
#include <blib/VBO.h>
#include <blib/VIO.h>

#define MAX_PARTICLES 10000

namespace blib
{
	class Renderer;
	class ResourceManager;
	class EmitterTemplate;
	class Emitter;
	class Shader;
	class SpriteBatch;
	class TextureMap;


	typedef VertexP2T2C4P2R1S1 VertexDef;

	class Particle
	{
	public:
		glm::vec2 position;
		glm::vec2 prevPosition;
		float life;
		float lifeDec;
		TextureMap::TexInfo* texture;
		float rotationSpeed;
		float rotation;

		VertexDef *vertex;
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
		double life;
		int lastTextureSelected;

		bool enabled;

		Emitter(EmitterTemplate* emitterTemplate);
		void newParticle(Particle& particle, double elapsedTime);
	};

	class Attractor
	{

	};

	class EmitterTemplate
	{
	public:
		EmitterTemplate(std::string filename, TextureMap* textureMap, const std::string &textureFolder);

		enum ParticleType
		{
				Fading
		} particleType;
		std::vector<std::string> textures;
		std::vector<TextureMap::TexInfo*> textureInfos;

		enum TextureOrder
		{
			Random,
			Ordered,
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

		glm::vec2 initialSpreadX;
		glm::vec2 initialSpreadY;

		struct ParticleProps
		{
			float directionMin;
			float directionMax; 

			float speedMin;
			float speedMax;

			float rotationMin;
			float rotationMax;

			float rotationSpeedMin;
			float rotationSpeedMax;

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
        std::map<std::string, EmitterTemplate*> cache;
		std::string textureFolder;
		double lastElapsedTime;
		Renderer* renderer;
		SpriteBatch* spriteBatch;
		Shader* shader;
		class ShaderUniforms
		{
		public:
			enum 
			{
				s_texture,
				projectionmatrix,
				matrix,
			};
		};

		std::list<Emitter*> emitters;
		TextureMap* textureMap;
	public:
		RenderState renderState;

		ParticleSystem(Renderer* renderer, ResourceManager* resourceManager, SpriteBatch* spriteBatch, int textureSize = 1024);
		~ParticleSystem();
		void update(double elapsedTime);
		void updateParticles(Particle* particles, int& nParticles, double elapsedTime);
		void draw(glm::mat4 matrix);


		Particle alphaParticles[MAX_PARTICLES];
		int nParticlesAlpha;
		Particle addParticles[MAX_PARTICLES];
		int nParticlesAdd;

		VertexDef* particleData;
		blib::VBO* vbo;
		blib::VIO* vio;

		void clear();
		void clearParticles();
		Emitter* addEmitter(std::string name);
		void removeEmitter(Emitter* emitter);

		Attractor* addAttractor();
		void removeAttractor();

		void preCache(std::string dir);

		inline void setTextureFolder(const std::string &folder) { this->textureFolder = folder; }

		void resizeGl( int width, int height, int offsetX, int offsetY );
	};


}
