#pragma once

#include <blib/Texture.h>

namespace blib
{

	class FBO : public Texture
	{
	public:
		bool freed;

		bool depth;
		bool depthTexture;
		bool stencil;
		int textureCount;

		FBO()
		{
			freed = false;
			stencil = false;
			depthTexture = false;
			textureCount = 1;
		}

		virtual void bind() = 0;
		virtual void setSize(int width, int height) = 0;
		virtual void free() { freed = true; };
	};


}
