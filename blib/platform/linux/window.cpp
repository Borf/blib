#include "window.h"
#include <blib/util/Log.h>
using blib::util::Log;


				Window					win;
				Window					root;


namespace blib
{
	namespace platform
	{
		namespace linux
		{
			Window::Window(App* app)
			{
			}
			Window::~Window()
			{
			}
			
			void Window::create(int icon, std::string title)
			{
				att[0] = GLX_RGBA;
				att[1] = GLX_DEPTH_SIZE;
				att[2] = 24;
				att[3] = GLX_DOUBLEBUFFER;
				att[4] = None;
			
				dpy = XOpenDisplay(NULL);
				if(!dpy)
				{
					Log::out<<"Cannot connect to the X server"<<Log::newline;
					exit(0);
				}
				
				root = DefaultRootWindow(dpy);
				
				vi = glXChooseVisual(dpy, 0, att);
				if(!vi)
				{
					Log::out<<"No proper visual found"<<Log::newline;
					exit(0);
				}
				
				cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
				
				swa.colormap = cmap;
				swa.event_mask = ExposureMask | KeyPressMask;
				
				win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
				XMapWindow(dpy, win);
				XStoreName(dpy, win, title.c_str());
	
	
	
				glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
				glXMakeCurrent(dpy, win, glc);
				
				
				glewInit();
				
			}
			
			void Window::makeCurrent()
			{
				glXMakeCurrent(dpy, win, glc);
			}
			
			void Window::unmakeCurrent()
			{
				glXMakeCurrent(dpy, 0, 0);

			}
			
			void Window::tick()
			{
				Log::out<<"WindowTick!"<<Log::newline;
				while(XPending(dpy) > 0)
				{
					XNextEvent(dpy, &xev);
		
					if(xev.type == Expose)
					{

					}
					if(xev.type == KeyPress)
					{
						exit(0);
					}
				}
			}
			
			void Window::swapBuffers()
			{
				glXSwapBuffers(dpy, win);
			}
		}
	}
}
