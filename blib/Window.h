#pragma once

#include <blib/config.h>

#if defined(BLIB_WIN)
#include <blib/platform/win32/window.h>
#elif defined(BLIB_LINUX)
#include <blib/platform/linux/window.h>
#elif defined(BLIB_ANDROID)
#include <blib/platform/android/window.h>
#else
#include <blib/IWindow.h>
#endif



namespace blib
{
	class App;

#if defined(BLIB_WIN)
	#define WINDOW_SUPER platform::win32::Window
#elif defined(BLIB_LINUX)
	#define WINDOW_SUPER platform::linux::Window
#elif defined(BLIB_ANDROID)
	#define WINDOW_SUPER platform::android::Window
#else
	class Window : public IWindow
	#define WINDOW_SUPER IWindow
#endif
	class Window : public WINDOW_SUPER
	{
	public:
		Window(App* app);
	};

}
