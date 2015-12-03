#pragma once

#include <blib/Resource.h>

namespace blib
{
	namespace gl { class Renderer; }
	class VIO : public Resource
	{
	protected:
		int elementSize;
	public:
		VIO() : Resource("VIO")
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

		friend class gl::Renderer;
	};
}