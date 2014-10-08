#pragma once

#include "Panel.h"

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class ScrollPanel : public Panel
			{
			public:
				int internalWidth;
				int internalHeight;

				int scrollX;
				int scrollY;

				ScrollPanel();

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;

				void arrangeComponents( int oldWidth, int oldHeight );

				virtual Widget* getComponent( int x, int y );
			};
		}
	}
}