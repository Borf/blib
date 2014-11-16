#pragma once

namespace blib
{
	class VBO
	{
	protected:
		int (*elementSize)();
	public:
		VBO()
		{
			elementSize = NULL;
		}
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