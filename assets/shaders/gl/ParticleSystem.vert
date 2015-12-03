attribute vec2 a_vposition;
attribute vec2 a_texture;
attribute vec4 a_color;
attribute vec2 a_position;
attribute float a_rotation;
attribute float a_scale;

varying vec2 texCoord;
varying vec4 color;
uniform mat4 matrix;
uniform mat4 projectionmatrix;

void main()
{
	color = a_color;
	texCoord = a_texture;

	mat2 rotationMatrix = mat2( cos(a_rotation), -sin(a_rotation),
								   sin(a_rotation),  cos(a_rotation));


	vec4 point = projectionmatrix * matrix * vec4(a_vposition + rotationMatrix * (a_position * a_scale),0,1);


	gl_Position = point;
}