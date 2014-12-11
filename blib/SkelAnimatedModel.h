#pragma once

#include <string>
#include <vector>
#include <map>
#include <blib/gl/Vertex.h>

namespace blib
{
	class VBO;
	class VIO;
	class RenderState;
	class Renderer;





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