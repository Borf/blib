#include "Resource.h"

#ifdef RESOURCECALLSTACK

#include <blib/Util.h>

namespace blib
{
	Resource::Resource()
	{
		callstack = blib::util::callstack();
	}

}
#endif