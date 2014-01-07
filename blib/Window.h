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
#if defined(BLIB_WIN)
	class Window : public platform::win32::Window
#elif defined(BLIB_LINUX)
	class Window : public platform::linux::Window
#elif defined(BLIB_ANDROID)
	class Window : public platform::android::Window
#else
	class Window : public IWindow
#endif
	{
	};

}
