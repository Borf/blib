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
			Window::Window()
			{
				create(0,"");
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
				
				win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
				XMapWindow(dpy, win);
				XStoreName(dpy, win, "BlaaaA");
	
	
	
				glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
				glXMakeCurrent(dpy, win, glc);
				
				
				while(true)
				{
					XNextEvent(dpy, &xev);
					
					if(xev.type == Expose)
					{
						glClearColor(1,0,0,1);
						glClear(GL_COLOR_BUFFER_BIT);
						glXSwapBuffers(dpy, win);
					}
					if(xev.type == KeyPress)
					{
						exit(0);
					}
				}
			}
		}
	}
}