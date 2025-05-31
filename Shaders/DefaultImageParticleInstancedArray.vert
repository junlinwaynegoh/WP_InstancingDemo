/*!
	Just a normal default shader
*/

#version 420

layout(location = 0) in vec2 vPos;
layout(location = 2) in vec2 aTexCoord;

//location 3,4,5,6 are reserved in this case
//so the next should start from pos 7
layout(location = 3) in mat4 MVP;
layout(location = 7) in float aImageIndex;

out vec2 oTexCoord;
out float oImageIndex;

void main()
{
	gl_Position = MVP * vec4(vPos,0.0,1.0);
	oTexCoord = aTexCoord;
	oImageIndex = aImageIndex;
}