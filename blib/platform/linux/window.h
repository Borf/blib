#pragma once

#include <GL/glew.h>
#include <blib/IWindow.h>
#include<stdio.h>
#include<stdlib.h>

#include<X11/X.h>
#include<X11/Xlib.h>

#include<GL/gl.h>
#include<GL/glx.h>






namespace blib
{
	class App;
	
	namespace platform
	{
		namespace linux
		{



			class Window : public blib::IWindow
			{
			private:
				int mouseButtons;
				std::vector<long> clicks;
				int lastButton;

				Display*				dpy;
				GLint					att[10];
				XVisualInfo*			vi;
				Colormap				cmap;
				GLXContext				glc;
				XWindowAttributes		gwa;
				XEvent					xev;
				XSetWindowAttributes	swa;
			protected:

			public:
				Window(App* app);
				virtual ~Window();
				virtual void create(int icon, std::string title);
				virtual void tick();
				virtual void makeCurrent();
				virtual void unmakeCurrent();
				virtual void swapBuffers();
			};
		}
	}
}