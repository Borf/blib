#include "window.h"
#include <unistd.h>

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
                Log::out<<"Creating window of " <<width <<" x " <<height<<Log::newline;
				win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
                XMapWindow(dpy, win);
                XStoreName(dpy, win, title.c_str());
                glc = glXCreateContext(dpy, vi, NULL, GL_FALSE);
                //makes the screen turn black
                glXMakeCurrent(dpy, win, glc);
				glewInit();

				int glVersion[2] = {-1, -1}; // Set some default values for the version
				glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
				glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using

				Log::out<<"Using OpenGL " << (const char*)glGetString(GL_VERSION) << Log::newline;
				Log::out<<"Using OpenGL " << glVersion[0] << "." << glVersion[1] << Log::newline; // Output which version of OpenGL we are using

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
