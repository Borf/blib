#include "window.h"

#include <gl/glew.h>
#include <gl/wglew.h>
#include <blib/util/Log.h>

using blib::util::Log;

namespace blib
{
	namespace gl
	{
		Window::Window()
		{
			showBorder = true;
			width = 1024;
			height = 768;
		}
		Window::~Window()
		{
		}


		void Window::create()
		{
			blib::Window::create();

			PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
			pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
			pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
			pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
			pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
			pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

			int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Check if our PFD is valid and get a pixel format back
			if (nPixelFormat == 0) // If it fails
				return;

			BOOL bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
			if (!bResult) // If it fails
				return;

			HGLRC tempOpenGLContext = wglCreateContext(hdc); // Create an OpenGL 2.1 context for our device context
			wglMakeCurrent(hdc, tempOpenGLContext); // Make the OpenGL 2.1 context current and active


			HGLRC hrc = tempOpenGLContext;

			GLenum error = glewInit(); // Enable GLEW
			if (error != GLEW_OK) // If GLEW fails
				return;

			int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // Set the MAJOR version of OpenGL to 3
				WGL_CONTEXT_MINOR_VERSION_ARB, 2, // Set the MINOR version of OpenGL to 2
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
				0
			};

			int glVersion[2] = {-1, -1}; // Set some default values for the version
			glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
			glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using

			Log::out<<"Using OpenGL " << (const char*)glGetString(GL_VERSION) << Log::newline;
			Log::out<<"Using OpenGL " << glVersion[0] << "." << glVersion[1] << Log::newline; // Output which version of OpenGL we are using

			ShowCursor(FALSE);
			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);

			glViewport(0,0,width, height);

			wglSwapIntervalEXT(0);

		}

		void Window::swapBuffers()
		{
			SwapBuffers(hdc);
		}


		void Window::tick()
		{
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
				if (msg.message == WM_QUIT) {
					opened = false; // Set running to false if we have a message to quit
				}
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

	}
}