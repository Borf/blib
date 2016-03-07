uniform sampler2D s_texture;
varying vec2 texCoord;
varying vec4 color;
varying vec4 colorOverlay;

void main()
{
	vec4 tex = color*texture2D(s_texture, texCoord);
	gl_FragColor.rgb = tex.rgb*(1-colorOverlay.a) + colorOverlay.rgb * colorOverlay.a;
	gl_FragColor.a = tex.a;
}
