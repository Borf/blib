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

				virtual void draw(gl::SpriteBatch &spriteBatch, glm::mat4 matrix);
				virtual void mousewheel( int direction, int x, int y );

				void arrangeComponents( int oldWidth, int oldHeight );

				virtual Widget* getComponent( int x, int y );

				virtual void mousedown( int x, int y );
				virtual void mouseup( int x, int y );
				virtual void mouseclick( int x, int y, int clickcount );
				virtual void mousedrag( int x, int y );

			};
		}
	}
}