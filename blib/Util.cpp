#include <blib/Util.h>
#include <blib/config.h>

#ifdef BLIB_WIN
#include <Windows.h>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <shlobj.h>
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
			sprintf(buf, "%f", number);
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
		static int posX = -1;
		static int posY = -1;
		BOOL CALLBACK enumProc(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM param)
		{
			MONITORINFO monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(monitor, &monitorInfo);
			if((monitorInfo.dwFlags & MONITORINFOF_PRIMARY) == 0)
			{
				posX = rect->left;
				posY = rect->top;
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
			SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath);
			return std::string(szPath);
#endif
		}

	}
}

