#pragma once


namespace blib
{
	class Shader;
	class Texture;

	class RenderState
	{
	public:
		Shader* activeShader;
		Texture* activeTexture;
		//blend state
		//


		static RenderState* activeRenderState;
	};


}