attribute vec2 a_position;
attribute vec4 a_color;
varying vec4 color;
uniform mat4 projectionmatrix;
uniform mat4 matrix;
void main()
{
	color = a_color;
	gl_Position = projectionmatrix * matrix * vec4(a_position,0.0,1.0);
}