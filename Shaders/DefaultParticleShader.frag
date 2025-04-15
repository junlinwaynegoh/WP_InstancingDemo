/*!
	Just a normal default shader
*/

#version 420

uniform vec3 color;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

void main()
{
	fragment = vec4(color,1.0);	
}
