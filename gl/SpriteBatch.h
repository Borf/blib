#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace gl
	{
		class Texture;

		class SpriteBatch
		{
		public:
			virtual void begin() = 0;
			virtual void end() = 0;

			virtual void draw(Texture* sprite, glm::mat4 transform) = 0;

		};



	}
}
