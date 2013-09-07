#pragma once

namespace blib
{
	namespace util
	{
		class NotCopyable
		{
		protected:
			NotCopyable() {}
		private:
			NotCopyable(const NotCopyable& other) { throw "Not Copyable"; }
			NotCopyable operator = (const NotCopyable& other) { throw "Not Copyable"; }
		};
	}
}