#pragma once

#include "Widget.h"
#include <string>


namespace blib
{
	class Texture;
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
				virtual void draw(SpriteBatch &shader, glm::mat4 matrix);

				void setTexture(Texture*);
				Texture* getTexture();

				bool showBorder;
				bool scaleAspect;
			};
		}
	}
}