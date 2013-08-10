#pragma once

#include "Widget.h"
#include <string>
class Texture;


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class Image : public Widget
			{
			protected:
				Texture* texture;
			public:
				Image(Texture* texture);

				virtual void draw(gl::SpriteBatch &shader);
			};
		}
	}
}