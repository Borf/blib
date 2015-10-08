#pragma once

#ifdef _DEBUG
#define RESOURCECALLSTACK
#endif

#include <string>

namespace blib
{

	class Resource
	{
	public:
		std::string name;
#ifdef RESOURCECALLSTACK
		std::string callstack;
#endif
		Resource(const std::string &name);
		virtual ~Resource() {};
	};

}