#include <blib/platform/android/window.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>

using blib::util::Log;

namespace blib
{
	namespace platform
	{
		namespace android
		{
			Window::Window()
			{
			}
			Window::~Window()
			{
			}


			void Window::create(int icon, std::string title)
			{

			}


			void Window::makeCurrent()
			{
			//	eglMakeCurrent();
			}
			void Window::unmakeCurrent()
			{
			}


		}
	}
}
