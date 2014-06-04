#pragma once

#include <blib/IWindow.h>

#include <blib/util/Log.h>
using blib::util::Log;


namespace blib
{
	class App;
	namespace platform
	{
		namespace ios
		{
			class Window : public blib::IWindow
			{
			private:
				App* app;
			public:
				Window(App* app);
				virtual ~Window();

				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();
                virtual void tick() { }
			};
		}
	}
}
