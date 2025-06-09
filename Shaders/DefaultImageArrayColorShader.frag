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
uniform vec3 uColorTint;

void main()
{
	fragment = vec4(uColorTint * vec3(texture(uTextureArray,vec3(oTexCoord, ulayerIndex))),1.f);	
}
