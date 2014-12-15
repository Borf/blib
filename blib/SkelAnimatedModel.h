#pragma once

#include <string>
#include <vector>
#include <map>
#include <blib/gl/Vertex.h>
#include <blib/Material.h>
#include <blib/util/Tree.h>
#include <glm/gtc/quaternion.hpp>

namespace blib
{
	class VBO;
	class VIO;
	class RenderState;
	class Renderer;
	class Texture;
	class ResourceManager;

	namespace json { class Value;  };



	class SkelAnimatedModel
	{
		class Bone : public blib::util::DynTree<Bone>
		{
		public:
			Bone(const json::Value& value);
			Bone* parent;
			std::string name;
			glm::mat4 matrix;

			int index;
			glm::mat4* offset;
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
				public:
					float time;
					T value;
				};

				Stream(const json::Value &data, Bone* rootBone);

				Bone* bone;
				std::vector<Frame<glm::vec3> >		positions;
				std::vector < Frame<glm::vec3> >	scales;
				std::vector< Frame<glm::quat> >		rotations;
			};

			Animation(const json::Value& data, Bone* rootBone);

			std::vector<Stream> streams;
			float totalTime;
			std::string name;
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
			SkelAnimatedModel* model;
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
		std::vector<Mesh*> meshes;

		Bone* rootBone;
		std::vector<Bone*> bones;
		std::map<std::string, Animation*> animations;
		std::vector<State*> states;

		void loadModel(const std::string & modelFile, blib::ResourceManager* resourceManager, blib::Renderer* renderer);
		void loadSkeleton(const std::string & boneFile);


	public:
		SkelAnimatedModel(const std::string &meshFile, const std::string &boneFile, blib::ResourceManager* resourceManager, blib::Renderer* renderer);
		void loadAnimation(const std::string &fileName);

		State* getNewState();
		void disposeState(State* state); // the state could just be deleted or copied too...
	};
}