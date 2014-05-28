#pragma once

#if defined(WIN32)
#include <blib/platform/win32/config.h>
#elif defined(ANDROID)
#include <blib/platform/android/config.h>
#elif __APPLE__

#include <blib/platform/ios/config.h>

#else
#include <blib/platform/linux/config.h>
#endif
