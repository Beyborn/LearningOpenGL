#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

uniform float hOffset;

out vec3 ourPosition;

void main()
{
   gl_Position = vec4(aPos.x + hOffset, aPos.y, aPos.z, 1);
   ourPosition = aPos;
}