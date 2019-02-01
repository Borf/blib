#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN)
#include <blib/platform/win32/config.h>
#elif defined(ANDROID)
#include <blib/platform/android/config.h>
#elif defined(__APPLE__)

#include <blib/platform/ios/config.h>

#elif defined(EMSCRIPTEN)
#include <blib/platform/emscripten/config.h>

#elif defined(_NX)
#include <blib/platform/nx/config.h>

#else
#include <blib/platform/linux/config.h>
#endif


#ifdef BLIB_OPENGL
#if !defined(BLIB_GL_ES) && !defined(BLIB_GL_FULL)

#error Please choose an OpenGL profile... ES or FULL

#endif
#endif