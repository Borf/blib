#include <blib/platform/ios/window.h>

#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>
#include <blib/App.h>
#include <blib/Util.h>

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
				int clickCount = 1;
				clicks.push_back(blib::util::tickcount());
				int i = clicks.size() - 2;
				while (i >= 0 && clicks[i] > clicks[i + 1] - 200)
				{
					i--;
					clickCount++;
				}
				if (clickCount < clicks.size())
					clicks.erase(clicks.begin(), clicks.begin() + clicks.size() - clickCount);

				
				for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseDown(x, y, MouseListener::Left, clickCount);
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
