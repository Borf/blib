precision highp float;

attribute vec2 a_position;
attribute vec2 a_texture;
attribute vec4 a_color;
attribute vec4 a_colorOverlay;

varying vec2 texCoord;
varying vec4 color;
varying vec4 colorOverlay;
uniform mat4 matrix;
uniform mat4 projectionmatrix;

void main()
{
	color = a_color;
	colorOverlay = a_colorOverlay;
	texCoord = a_texture;
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);
}