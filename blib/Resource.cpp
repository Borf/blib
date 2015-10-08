#include "Resource.h"

#ifdef RESOURCECALLSTACK

#include <blib/Util.h>
#endif

namespace blib
{
	Resource::Resource(const std::string &name) : name(name)
	{
#ifdef RESOURCECALLSTACK
		callstack = blib::util::callstack();
#endif
	}

}
