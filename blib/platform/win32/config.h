#pragma once

#define BLIB_WIN

#define BLIB_OPENGL
#define BLIB_DIRECTX

#define BLIB_GL_FULL

#define BLIB_WINMM

#if !defined(__CYGWIN)
#define BLIB_DIRECTINPUT
#define BLIB_VISUALSTUDIO
#endif

