#pragma once

#include "Widget.h"

#include <string>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class Button : public Widget
			{
			public:
				std::string text;

				Button(std::string text);

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
			};
		}
	}
}
