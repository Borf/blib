#pragma once

#include "ContainerWidget.h"

namespace blib
{
	namespace wm
	{
		namespace widgets
		{

			class Panel : public ContainerWidget
			{
			public:
				virtual void draw(SpriteBatch& spriteBatch, glm::mat4 matrix);
				void arrangeComponents( int oldWidth, int oldHeight );
			};
		}
	}
}
