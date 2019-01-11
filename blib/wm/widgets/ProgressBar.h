#pragma once

#include "Widget.h"

#include <string>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class ProgressBar : public Widget
			{
			public:
				float min;
				float max;
				float value;

				ProgressBar();

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
			};
		}
	}
}