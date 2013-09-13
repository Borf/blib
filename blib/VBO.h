#pragma once

#include <blib/util/FastDelegate.h>

namespace blib
{
	class VBO
	{
	protected:
		int (*elementSize)();
	public:


		virtual void bind() = 0;
		virtual void setData(int length, void* data) = 0;
		virtual int getLength() = 0;
		
		template<class T>
		void setVertexFormat()
		{
			elementSize = &T::size;
		}
	};
}