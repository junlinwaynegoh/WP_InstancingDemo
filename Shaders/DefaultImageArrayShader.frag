/*!
	Just a normal default shader
*/

#version 420

//from vertex shader
in vec2 oTexCoord;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

//image texture
uniform sampler2DArray uTextureArray;
uniform int ulayerIndex;

void main()
{
	fragment = texture(uTextureArray,vec3(oTexCoord, ulayerIndex));	
}
