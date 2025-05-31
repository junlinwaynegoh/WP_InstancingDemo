/*!
	Just a normal default shader
*/

#version 420

uniform mat4 MVP;

layout(location = 0) in vec2 vPos;
layout(location = 2) in vec2 aTexCoord;

out vec2 oTexCoord;

void main()
{
	gl_Position = MVP * vec4(vPos,0.0,1.0);
	oTexCoord = aTexCoord;
}