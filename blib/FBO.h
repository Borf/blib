#pragma once

#include <blib/Texture.h>

namespace blib
{

	class FBO : public Texture
	{
	public:
		virtual void bind() = 0;
		virtual void setSize(int width, int height) = 0;
	};


}
