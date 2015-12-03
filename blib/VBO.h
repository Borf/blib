#pragma once

#include <blib/Resource.h>

namespace blib
{
	class VBO : public Resource
	{
	protected:
		int (*elementSize)();
	public:
		VBO() : Resource("VBO")
		{
			elementSize = NULL;
		}
		virtual void bind() = 0;
		virtual void setData(int length, void* data) = 0;
		virtual void setSubData(int offset, int length, void* data) = 0;
		virtual int getLength() = 0;
		
		template<class T>
		void setVertexFormat()
		{
			elementSize = &T::size;
		}
	};
}