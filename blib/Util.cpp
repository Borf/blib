#include <blib/Util.h>
#include <blib/config.h>

#ifdef BLIB_WIN
#include <Windows.h>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <shlobj.h>
#include <DbgHelp.h>
#pragma comment(lib,"dbghelp.lib")
#include <winbase.h>
#else
#include <execinfo.h>
#include <sys/time.h>
#endif

#ifndef NAN
#define _HUGE_ENUF  1e+300	/* _HUGE_ENUF*_HUGE_ENUF must overflow */
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))  /* causes warning C4756: overflow in constant arithmetic (by design) */
#define NAN        ((float)(INFINITY * 0.0F))

#endif


namespace blib
{
	namespace util
	{
		glm::vec2 fromAngle( float angle )
		{
			return glm::vec2(cos(angle), sin(angle));
		}

		float wrapAngleRad(float angle)
		{
			while(angle > glm::radians(180.0f))
				angle -= glm::radians(360.0f);
			while(angle < -glm::radians(180.0f))
				angle += glm::radians(360.0f);
			return angle;
		}

		float wrapAngleDeg(float angle)
		{
			while(angle > 180.0f)
				angle -= 360.0f;
			while(angle < -180.0f)
				angle += 360.0f;
			return angle;
		}


		std::string toString( int number )
		{
			char buf[32];
			sprintf(buf, "%d", number);
			return buf;
		}
		std::string toString( float number )
		{
			char buf[32];
			sprintf(buf, "%g", number);
			
/*			bool period = -1;
			int lastDigit = strlen(buf);
			for (int i = strlen(buf) - 1; i >= 0 && period == -1; i--)
			{
				if (!period && buf[i] == '0')
					lastDigit = i;
				else if (buf[i] == '.')
					period = i;
			}
			if (period != -1)
			{
				buf[lastDigit] = 0;
			}
			*/
			return buf;
		}
		std::vector<std::string> split( std::string value, std::string seperator )
		{
			std::vector<std::string> ret;
			while(value.find(seperator) != std::string::npos)
			{
				int index = value.find(seperator);
				if(index != 0)
					ret.push_back(value.substr(0, index));
				value = value.substr(index+seperator.length());
			}
			ret.push_back(value);
			return ret;
		}

		std::string replace(std::string orig, const std::string &find, const std::string &replace)
		{
			throw "argh";
		}


		bool compareVec4::operator()( const glm::vec4 &a, const glm::vec4 &b ) const
		{
			if(a.x != b.x)
				return a.x < b.x;
			if(a.y != b.y)
				return a.y < b.y;
			if(a.z != b.z)
				return a.z < b.z;
			if(a.w != b.w)
				return a.w < b.w;
			return false;
		}

#ifdef WIN32
		RECT screenRect;
		static int posX = -1;
		static int posY = -1;
		BOOL CALLBACK enumProc(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM param)
		{
			MONITORINFO monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(monitor, &monitorInfo);
			if ((monitorInfo.dwFlags & MONITORINFOF_PRIMARY) == 0)
			{
				posX = rect->left;
				posY = rect->top;
			}
			return TRUE;
		}
		BOOL CALLBACK enumProc2(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM param)
		{
			MONITORINFO monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(monitor, &monitorInfo);
			if ((monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0)
			{
				screenRect = *rect;
			}
			return TRUE;
		}

		HWND GetConsoleHwnd()
		{
			HWND hwndFound;
			char TempWindowTitle[1024];
			char WindowTitle[1024];
			GetConsoleTitleA(WindowTitle, 1024);
			sprintf_s(TempWindowTitle,1024, "%d/%d", GetTickCount(), GetCurrentProcessId());
			SetConsoleTitleA(TempWindowTitle);
			Sleep(10);
			hwndFound=FindWindowA(NULL, TempWindowTitle);
			SetConsoleTitleA(WindowTitle);
			return(hwndFound);
		}



#endif

		glm::ivec2 getResolution()
		{
#ifdef WIN32
			EnumDisplayMonitors(NULL, NULL, enumProc2, NULL);

			return glm::ivec2(screenRect.right - screenRect.left, screenRect.bottom - screenRect.top);
#else
			return glm::ivec2(1920, 1080);
#endif
		}
		void fixConsole()
		{
#ifdef WIN32
			if(GetSystemMetrics(80) > 1)
			{
				EnumDisplayMonitors(NULL, NULL, enumProc, NULL);
				SetWindowPos(GetConsoleHwnd(), GetConsoleHwnd(), posX, posY,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
#endif
		}

		std::string getDataDir()
		{
#ifdef BLIB_WIN
            TCHAR szPath[MAX_PATH];
			ZeroMemory(szPath, MAX_PATH);
            SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath);

#ifdef UNICODE
            size_t size = wcslen(szPath);
            char * buffer = new char [2*size+2];
            wcstombs(buffer,szPath,2*size+2);
            std::string string(buffer);
            delete [] buffer;
            return string;
#else
            return std::string(szPath);
#endif
#endif
return "~/";
		}

		glm::vec3 hsv2rgb(glm::vec3 hsv)
		{
			double      hh, p, q, t, ff;
			long        i;

			if (hsv.y <= 0.0) {       // < is bogus, just shuts up warnings
				return glm::vec3(hsv.z, hsv.z, hsv.z);
			}
			hh = hsv.x;
			while (hh >= 360.0) hh = 0.0;
			hh /= 60.0;
			i = (long)hh;
			ff = hh - i;
			p = hsv.z * (1.0 - hsv.y);
			q = hsv.z * (1.0 - (hsv.y * ff));
			t = hsv.z * (1.0 - (hsv.y * (1.0 - ff)));

			switch (i) {
			case 0:
				return glm::vec3(hsv.z, t, p);
			case 1:
				return glm::vec3(q, hsv.z, p);
			case 2:
				return glm::vec3(p, hsv.z, t);
			case 3:
				return glm::vec3(p, q, hsv.z);
			case 4:
				return glm::vec3(t, p, hsv.z);
			case 5:
			default:
				return glm::vec3(hsv.z, p, q);
			}
		}


		glm::vec3 rgb2hsv(glm::vec3 rgb)
		{
			glm::vec3         out;
			float      min, max, delta;

			min = rgb.r < rgb.g ? rgb.r : rgb.g;
			min = min  < rgb.b ? min : rgb.b;

			max = rgb.r > rgb.g ? rgb.r : rgb.g;
			max = max  > rgb.b ? max : rgb.b;

			out.z = max;                                // v
			delta = max - min;
			if (max > 0.0) {
				out.s = (delta / max);                  // s
			}
			else {
				// r = g = b = 0                        // s = 0, v is undefined
				out.y = 0.0;
				out.x = NAN;                            // its now undefined
				return out;
			}
			if (rgb.r >= max)                           // > is bogus, just keeps compilor happy
				out.x = (rgb.g - rgb.b) / delta;        // between yellow & magenta
			else
				if (rgb.g >= max)
					out.x = 2.0f + (rgb.b - rgb.r) / delta;  // between cyan & yellow
				else
					out.x = 4.0f + (rgb.r - rgb.g) / delta;  // between magenta & cyan

			out.x *= 60.0;                              // degrees

			if (out.x < 0.0)
				out.x += 360.0;

			return out;
		}

		std::string callstack()
		{
#ifdef WIN32
			std::string ret;
			unsigned int   i;
			void         * stack[100];
			unsigned short frames;
			SYMBOL_INFO  * symbol;
			HANDLE         process;

			process = GetCurrentProcess();

			SymInitialize(process, NULL, TRUE);

			frames = CaptureStackBackTrace(0, 100, stack, NULL);
			symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO)+256 * sizeof(char), 1);
			symbol->MaxNameLen = 255;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			for (i = 0; i < frames; i++)
			{
				SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
				char buf[1024];
				sprintf(buf, "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
				ret += buf;
			}
			SymCleanup(process);
			free(symbol);

			return ret;
#else
            std::string ret;
            void* callstack[128];
            int frames= backtrace(callstack, 128);
            char** strs = backtrace_symbols(callstack, frames);
            for(int i = 0; i < frames; i++)
            {
                ret += std::string(strs[i]) + "\n";
            }
            free(strs);
            return ret;
#endif
			return "";
		}

		long tickcount()
		{
#ifdef WIN32
			return GetTickCount();
#else
			struct timeval tv;
			if (gettimeofday(&tv, NULL) != 0)
				return 0;

			return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
		}

		bool isMainModule()
		{
#ifdef BLIB_WIN
			static int ret = -1;
			if (ret == -1)
			{
				HMODULE hModule = NULL;
				GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)isMainModule, &hModule);
				FARPROC proc = GetProcAddress(hModule, "main");

				ret = proc != NULL ? 1 : 0;
			}
			return ret == 1;
#else
            return true;
#endif
		}

	}
}

