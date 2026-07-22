#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

uniform float hOffset;

out vec3 ourColour;
out vec2 texCoord;

void main()
{
   gl_Position = vec4(aPos.x + hOffset, aPos.y, aPos.z, 1);
   ourColour = aColour;
   texCoord = vec2(aTexCoord.x, aTexCoord.y);
}