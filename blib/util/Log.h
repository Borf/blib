#pragma once

#include <string>
#include <blib/config.h>
#ifdef BLIB_WIN
#include <Windows.h>
#endif




namespace blib
{
	namespace util
	{
#ifdef BLIB_WIN
		namespace ConsoleForeground
		{
			enum {
				BLACK = 0,
				DARKBLUE = FOREGROUND_BLUE,
				DARKGREEN = FOREGROUND_GREEN,
				DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
				DARKRED = FOREGROUND_RED,
				DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
				DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
				DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
				GRAY = FOREGROUND_INTENSITY,
				BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
				GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
				CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
				RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
				MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
				YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
				WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			};
		}
#endif

		class Log
		{


			std::string buffer;
		//	bool endline;
			class EndLine{};

			void logString(const char* format, ...);

#ifdef BLIB_WIN
			static HANDLE handle;
#endif

		public:
			Log();

			static EndLine newline;

			Log& operator <<(const char* txt);
			Log& operator <<(std::string txt);
			Log& operator <<(int txt);
			Log& operator <<(unsigned long txt);
			Log& operator <<(unsigned int txt);
			Log& operator <<(float txt);
			Log& operator <<(double txt);
			Log& operator <<(const EndLine& endline);


			static std::string format(const char* fmt, ...); //TODO: move this to a better spot
			static Log out;
			static Log err;
		};
	}
}


