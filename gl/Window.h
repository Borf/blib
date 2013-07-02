#pragma once

#include <blib/Window.h>

namespace blib
{
	namespace gl
	{
		class Window : public blib::Window
		{
		protected:
			int width,height;
			bool resizable;
		public:
			Window();
			~Window();
			virtual void setSize( int width, int height );
			virtual void isResizable( bool resizable );
			virtual void create();
		private:
			Window(const Window& other) {};

		};
	}
}