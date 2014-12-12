#pragma once

#include <string>
#include <vector>
#include <map>
#include <blib/gl/Vertex.h>
#include <blib/Shader.h>
#include <blib/Material.h>
#include <glm/gtc/quaternion.hpp>

namespace blib
{
	class VBO;
	class VIO;
	class RenderState;
	class Renderer;
	class Texture;




	class SkelAnimatedModel
	{
		class Bone
		{
		public:
			int index;
		};

		class Animation
		{
		public:
			class Stream
			{
			public:
				template<class T>
				class Frame
				{
					float time;
					T value;
				};

				Bone* bone;
				std::vector<Frame<glm::vec3> >		positions;
				std::vector < Frame<glm::vec3> >	scales;
				std::vector< Frame<glm::quat> >		rotations;
			};


			std::vector<Stream> streams;
		};

		class Mesh
		{
		public:
			Material material;
			int begin;
			int count;
		};


	public:
		class State
		{
		public:
			SkelAnimatedModel* mesh;
			double time;
			std::vector<glm::mat4> boneMatrices;
			Animation* currentAnimation;


			void setAnimation(const std::string& animation);
			void update(float elapsedTime);
			void draw(RenderState& renderState, Renderer* renderer, int materialUniform, int boneUniform);
		};
	private:


		std::vector<blib::VertexP3T2N3B4B4> vertices;


		blib::VBO* vbo;
		blib::VIO* vio;

		Bone* rootBone;
		std::map<std::string, Animation*> animations;



		std::vector<State*> states;
	public:
		SkelAnimatedModel(const std::string &meshFile, const std::string &boneFile);
		void loadAnimation(const std::string fileName);

		State* getNewState();
		void disposeState(State* state); // the state could just be deleted or copied too...
	};
}