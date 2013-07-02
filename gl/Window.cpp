#include "window.h"

namespace blib
{
	namespace gl
	{
		void Window::setSize( int width, int height )
		{
			this->width = width;
			this->height = height;
		}

		void Window::isResizable( bool resizable )
		{
			this->resizable = resizable;
		}

		void Window::create()
		{
			
		}
	}
}