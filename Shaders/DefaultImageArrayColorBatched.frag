/*!
	Just a normal default shader
*/

#version 420

//from vertex shader
in vec2 oTexCoord;
in vec3 oColor;
in float oImageIndex;

//the resulting fragment/(color of the pixel)
out vec4 fragment;

//image texture
uniform sampler2DArray uTextureArray;

void main()
{
	fragment = vec4(oColor * vec3(texture(uTextureArray,vec3(oTexCoord, oImageIndex))),1.f);	
}
