precision highp float;

attribute vec2 a_position;
attribute vec2 a_texture;
attribute vec4 a_color;
varying vec2 texCoord;
varying vec4 color;
uniform mat4 matrix;
uniform mat4 projectionmatrix;

void main()
{
	color = a_color;
	texCoord = a_texture;
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);
}