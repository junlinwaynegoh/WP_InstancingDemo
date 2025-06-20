/*!
	Just a normal default shader
*/

#version 420

//from vertex shader
in vec2 oTexCoord;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

//image texture
uniform sampler2D uTexture;

void main()
{
	fragment = texture(uTexture,oTexCoord);	
}
