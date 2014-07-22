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
            
            void Window::touchDownEvent(int x, int y)
            {
                for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
                    (*it)->onMouseDown(x,y,MouseListener::Left, 1);
            }
            
            void Window::touchUpEvent(int x, int y)
            {
                for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
                    (*it)->onMouseUp(x,y,MouseListener::Left, 1);
            }
            void Window::touchMoveEvent(int x, int y)
            {
                for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
                    (*it)->onMouseMove(x,y,MouseListener::Left);
            }
            
		}
	}
}
