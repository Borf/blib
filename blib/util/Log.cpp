#ifdef WIN32
#include <blib/linq.h>
#endif

#include "Log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include <iostream>
#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <blib/util/Thread.h>
#include <blib/util/Mutex.h>
#endif


namespace blib
{
	namespace util
	{
		Log::EndLine Log::newline;
		Log Log::out;
		Log Log::err;


		#ifdef ANDROID
		#define  LOG_TAG    "blib"
		#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
		#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
		#endif

#ifdef WIN32
		static Mutex* logMutex = new Mutex();
#endif
		Log logger;

		Log::Log()
		{
			//endline = true;
		}
//TODO: make a buffer per threadId
		void Log::logString(const char* fmt, ...)
		{
			char text[10240];
			va_list ap;
			if (fmt == NULL)
				return;

		#ifdef WIN32
			va_start(ap,fmt);
			vsprintf_s(text,10240,fmt,ap);
			va_end(ap);
		#else
			va_start(ap,fmt);
			vsprintf(text,fmt,ap);
			va_end(ap);
		#endif
			buffer += text;
		//	std::cout<<text;
		}




		Log& Log::operator<<(const char* txt)
		{
			logString("%s", txt);
			return *this;
		}

		Log& Log::operator<<(int txt)
		{
			logString("%i", txt);
			return *this;
		}

		Log& Log::operator<<(unsigned long txt)
		{
			logString("%lu", txt);
			return *this;
		}

		Log& Log::operator<<(unsigned int txt)
		{
			logString("%u", txt);
			return *this;
		}

		Log& Log::operator<<(float txt)
		{
			logString("%f", txt);
			return *this;
		}

		Log& Log::operator<<(double txt)
		{
			logString("%f", txt);
			return *this;
		}


		Log& Log::operator<<(std::string txt)
		{
			logString("%s", txt.c_str());
			return *this;
		}


		Log& Log::operator<<(const EndLine& e)
		{
			//logString("%s", "\n");
			//endline = true;

			SYSTEMTIME beg;
			GetLocalTime(&beg);
			buffer = format( "[%02d:%02d:%02d:%03d]\t", beg.wHour, beg.wMinute, beg.wSecond, beg.wMilliseconds) + buffer;

#ifdef WIN32
			buffer = format("[%20s]",blib::util::Thread::getCurrentThreadName().c_str()) + buffer;
#endif


		#ifdef WIN32
			OutputDebugStringA(buffer.c_str());
			OutputDebugStringA("\r\n");
		#endif

		#ifdef ANDROID
			LOGI("%s", buffer.c_str());
		#else
			printf("%s\r\n", buffer.c_str());
		#endif
			buffer = "";
			return *this;
		}

		std::string Log::format(const char* fmt, ...)
		{
			char text[10240];
			va_list ap;
			if (fmt == NULL)
				return "";
		#ifdef WIN32
			va_start(ap,fmt);
			vsprintf_s(text,10240,fmt,ap);
			va_end(ap);
		#else
			va_start(ap,fmt);
			vsprintf(text,fmt,ap);
			va_end(ap);
		#endif
	
			return text;
		}


	}
}
