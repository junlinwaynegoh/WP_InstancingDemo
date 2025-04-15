/*!
	Just a normal default shader
*/

#version 420

layout(location = 0) in vec2 vPos;

//we dont start from 1 as from the initial setting up of
//primitive location = 1 is in color binding
//reference wp_primitive.cpp vposbindloc and vcolbinLoc

//location 2,3,4,5 are reserved in this case
//so the next should start from pos 6
layout(location = 2) in mat4 MVP;

//for example
//to set color
layout(location = 6) in vec3 color;

out vec3 fragCol;

void main()
{
	gl_Position = MVP * vec4(vPos,0.0,1.0);
	fragCol = color;
}