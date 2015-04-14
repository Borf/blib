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
		class Animation;
		class Bone : public blib::util::DynTree<Bone>
		{
		public:
			Bone(const json::Value& value);
			void update(std::vector<glm::mat4> &boneMatrices, float time, Animation* animation, const glm::mat4& parentMatrix = glm::mat4()) const;
			Bone* parent;
			std::string name;
			glm::mat4 matrix;

			int index;
			glm::mat4* offset;

			glm::mat4 getMatrix(Animation* animation, float time) const;
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
			Animation::Stream* getStream(const SkelAnimatedModel::Bone* bone);
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
			class AnimationState
			{
			public:
				double time;
				Animation* animation;
				float blendFactor;
				int playCount;
			};
			class Fader
			{
			public:
				AnimationState* animationState;
				float begin;
				float end;
				float time;
				float elapsedTime;
				bool stopWhenDone;
			};


			SkelAnimatedModel* model;
			std::vector<glm::mat4> boneMatrices;
			std::vector<AnimationState*> animations;
			std::vector<Fader*> faders;


			void playAnimation(const std::string& animation, float fadeInTime = 0, bool playOnce = false);
			void stopAnimation(const std::string& animation, float fadeOutTime = 0);
			void update(float elapsedTime);
			void draw(RenderState renderState, Renderer* renderer, int materialUniform, int boneUniform) const;
			void drawSkeleton(blib::RenderState renderState, Renderer* renderer);
		};
	private:


		std::vector<blib::VertexP3T2N3B4B4> vertices;


		blib::VBO* vbo;
		blib::VIO* vio;

		Bone* rootBone;
		std::vector<Bone*> bones;
		std::map<std::string, Animation*> animations;
		std::vector<State*> states;

		void loadModel(const std::string & modelFile, blib::ResourceManager* resourceManager, blib::Renderer* renderer);
		void loadSkeleton(const std::string & boneFile);


	public:
		std::vector<Mesh*> meshes;


		SkelAnimatedModel(const std::string &meshFile, const std::string &boneFile, blib::ResourceManager* resourceManager, blib::Renderer* renderer);
		void loadAnimation(const std::string &fileName);

		State* getNewState();
		void disposeState(State* state); // the state could just be deleted or copied too...
	};
}