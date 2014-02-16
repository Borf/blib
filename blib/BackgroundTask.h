#pragma once

#include <functional>
#include <blib/util/Thread.h>

namespace blib
{
	class App;

	class BackgroundTask : public util::Thread
	{
		App* app;
		std::function<void()> toRun;
		std::function<void()> whenDone;

	public:
		BackgroundTask(App* app, std::function<void()> toRun, std::function<void()> whenDone = [](){});
		virtual int run();

	};

}
