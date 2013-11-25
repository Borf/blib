#pragma once

#include <blib/IWindow.h>




namespace blib
{
	namespace platform
	{
		namespace linux
		{
			class Window : public blib::IWindow
			{
			private:
				int mouseButtons;
				std::vector<long> clicks;
				int lastButton;
			protected:

			public:
				Window();
				virtual ~Window();
				virtual void create(int icon, std::string title);
			};
		}
	}
}