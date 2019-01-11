#include "Window.h"

#if defined(BLIB_IOS)
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#elif defined(BLIB_ANDROID)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
	#ifdef WIN32
	#include <GL/wglew.h>
	#endif
#endif

#include <blib/gl/GlResizeRegister.h>
#include <blib/util/Log.h>
using blib::util::Log;

namespace blib
{
	namespace gl
	{
		Window::Window(App* app) : blib::Window(app)
		{
			showBorder = true;
			width = 1024;
			height = 768;
		}
		Window::~Window()
		{
			Log::out<<"GlWindow Destroyed"<<Log::newline;
		}
		
#ifdef WIN32		
		void APIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#else
		void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
		{
			if (type == 33361) //VBO setdata
				return;
			util::Log::out<<message<<Log::newline;
		}

		void Window::create(int icon, std::string title)
		{
			blib::Window::create(icon, title);
#ifdef WIN32
			PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
			pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
			pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
			pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
			pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
			pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD
			pfd.cStencilBits = 16;

			int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Check if our PFD is valid and get a pixel format back
			if (nPixelFormat == 0) // If it fails
			{
				Log::out<<"Error getting pixel format"<<Log::newline;
				return;
			}

			BOOL bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
			if (!bResult) // If it fails
			{
				Log::out<<"Error setting pixel format"<<Log::newline;
				return;
			}
			HGLRC tempOpenGLContext = wglCreateContext(hdc); // Create an OpenGL 2.1 context for our device context
			wglMakeCurrent(hdc, tempOpenGLContext); // Make the OpenGL 2.1 context current and active


			hrc = tempOpenGLContext;

			GLenum error = glewInit(); // Enable GLEW
			if (error != GLEW_OK) // If GLEW fails
			{
				Log::out<<"Error initializing glew"<<Log::newline;
				return;
			}

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

			Log::out << "Renderer: " << (const char*)glGetString(GL_RENDERER) << Log::newline;

//			ShowCursor(FALSE);
			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
#else




#endif

#ifdef __GLEW_H__
#ifdef GL_DEBUG_OUTPUT
#ifdef _DEBUG
			if(GLEW_ARB_debug_output)
			{
				glDebugMessageCallback(&onDebug, NULL);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
				glEnable(GL_DEBUG_OUTPUT);
			}
#endif
#endif
#endif
			Log::out<<"glViewPort"<<Log::newline;
			glViewport(0,0,width, height);
			Log::out<<"done glViewPort"<<Log::newline;


		}


		void Window::tick()
		{
#ifdef WIN32
			MSG msg;
			while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
				if (msg.message == WM_QUIT) {
					opened = false; // Set running to false if we have a message to quit
				}
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
#else
			blib::Window::tick();
#endif
		}

		void Window::onResize(int width, int height, int offX, int offY)
		{			
			blib::gl::GlResizeRegister::ResizeRegisteredObjects(width, height, offX, offY);
		}

	}
}
