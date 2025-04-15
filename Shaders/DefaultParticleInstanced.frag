/*!
	Just a normal default shader
*/

#version 420

in vec3 fragCol;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

void main()
{
	fragment = vec4(fragCol,1.0);	
}
