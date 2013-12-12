#pragma once

#if defined(WIN32)
#include <blib/platform/win32/config.h>
#elif defined(ANDROID)
#include <blib/platform/android/config.h>
#else
#include <blib/platform/linux/config.h>
#endif
