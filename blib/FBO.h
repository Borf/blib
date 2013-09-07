#pragma once

#include <blib/Texture.h>

namespace blib
{

	class FBO : public Texture
	{
	public:
		virtual void bind() = 0;
	};


}
