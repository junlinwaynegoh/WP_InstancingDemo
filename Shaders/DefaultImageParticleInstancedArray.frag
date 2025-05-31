/*!
	Just a normal default shader
*/

#version 420

in vec2 oTexCoord;
in float oImageIndex;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

//image texture
uniform sampler2DArray uTextureArray;

void main()
{
	fragment = texture(uTextureArray,vec3(oTexCoord, oImageIndex));	
}
