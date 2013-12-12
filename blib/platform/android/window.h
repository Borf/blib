#pragma once

#include <blib/IWindow.h>


#ifdef WIN32
#include <Windows.h>
#endif


namespace blib
{
	namespace platform
	{
		namespace android
		{
			class Window : public blib::IWindow
			{
			private:
			protected:
			public:
				Window();
				virtual ~Window();


				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();
			};
		}
	}
}
