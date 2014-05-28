#include <blib/platform/ios/window.h>

#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>
#include <blib/App.h>

using blib::util::Log;

namespace blib
{
	namespace platform
	{
		namespace ios
		{

			Window::Window(App* app)
			{
				this->app = app;
			}
			Window::~Window()
			{
			}


			void Window::create(int icon, std::string title)
			{
			}

			void Window::makeCurrent()
			{
			}
			void Window::unmakeCurrent()
			{
			}
			void Window::swapBuffers()
			{
			}



		}
	}
}
