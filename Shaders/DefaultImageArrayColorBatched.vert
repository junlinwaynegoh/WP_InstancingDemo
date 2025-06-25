/*!
	Just a normal default shader
*/

#version 420

layout(location = 0) in vec2 vPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in int aImageIndex;

out vec2 oTexCoord;
out vec3 oColor;
out flat int oImageIndex;

void main()
{
	gl_Position = vec4(vPos,0,1);
	oTexCoord = aTexCoord;
	oColor = aColor;
	oImageIndex = aImageIndex;
}