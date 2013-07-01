#pragma once

#ifdef BLIB_EXPORT
#define BLIB_API __declspec(dllexport)
#else
#define BLIB_API __declspec(dllimport)
#endif

namespace blib
{

}