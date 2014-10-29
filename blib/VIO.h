#pragma once


namespace blib
{
	class VIO
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
		void setVertexFormat()
		{
			elementSize = sizeof(T);
		}


	};
}