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
#endif

Log::EndLine Log::newline;


#ifdef ANDROID
#define  LOG_TAG    "pewpew"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

Log::Log()
{
	//endline = true;
}

void Log::logString(const char* fmt, ...)
{
/*	if(endline)
	{
		endline = false;

//		SYSTEMTIME beg;
//		GetLocalTime(&beg);
//		logString( "[%02d:%02d:%02d:%03d]\t", beg.wHour, beg.wMinute, beg.wSecond, beg.wMilliseconds);
	}*/

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
