precision highp float;
uniform sampler2D s_texture;
varying vec2 texCoord;
varying vec4 color;

void main()
{
	gl_FragColor = color*texture2D(s_texture, texCoord);
}
