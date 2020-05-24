#pragma once

#include "Widget.h"
#include <string>
#include <blib/TextureMap.h>

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
				TextureMap::TexInfo* texInfo;
			public:
				Image(Texture* texture);
				Image(TextureMap::TexInfo* texInfo);

				virtual void draw(SpriteBatch &shader, glm::mat4 matrix, Renderer* renderer) const;

				void setTexture(Texture*);
				Texture* getTexture();

				bool showBorder;
				bool scaleAspect;

				glm::vec4 bgcolor = glm::vec4(1, 1, 1, 1);
			};
		}
	}
}