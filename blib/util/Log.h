#pragma once

#include <string>
#include <list>
#include <blib/config.h>
#ifdef BLIB_WIN
#include <Windows.h>
#endif
#include <blib/json.hpp>



namespace blib
{
	namespace util
	{
		class Log
		{
		public:
			enum class Color;
		private:
			std::string buffer;
			Color color;
		//	bool endline;
			class EndLine{};

			void logString(const char* format, ...);

#ifdef BLIB_WIN
			static HANDLE handle;
#endif

		public:
			std::list<std::string> lines;
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
			Log& operator <<(const json &j);



#ifdef BLIB_WIN
			enum class Color {
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
#else
			enum class Color
			{
				BLACK,
				WHITE,
				RED,
			};
#endif
			Log& operator <<(Color color);

			static std::string format(const char* fmt, ...); //TODO: move this to a better spot
			static Log out;
			static Log err;
		};
	}
}


