#pragma once

#include "Widget.h"

#include <string>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class CloseButton : public Widget
			{
			public:
				CloseButton(std::string text);

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
			};
		}
	}
}
