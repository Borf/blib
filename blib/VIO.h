#pragma once

#include <blib/Resource.h>

namespace blib
{
	class VIO : public Resource
	{
	protected:
		int elementSize;
	public:
		VIO()
		{
			elementSize = 0;
		}

		virtual void bind() = 0;
		virtual void setData(int length, void* data) = 0;
		virtual int getLength() = 0;

		template<class T>
		void setElementType()
		{
			elementSize = sizeof(T);
		}


	};
}