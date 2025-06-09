/*!
	Just a normal default shader
*/

#version 420

in vec2 oTexCoord;
in float oImageIndex;
in vec3 oColorTint;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

//image texture
uniform sampler2DArray uTextureArray;

void main()
{
	fragment = vec4(oColorTint * vec3(texture(uTextureArray,vec3(oTexCoord, oImageIndex))),1.f);	
}
