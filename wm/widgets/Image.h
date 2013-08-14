#pragma once

#include "Widget.h"
#include <string>


namespace blib
{
	namespace gl { class Texture; }
	namespace wm
	{
		namespace widgets
		{
			class Image : public Widget
			{
			protected:
				gl::Texture* texture;
			public:
				Image(gl::Texture* texture);

				virtual void draw(gl::SpriteBatch &shader, glm::mat4 matrix);
			};
		}
	}
}