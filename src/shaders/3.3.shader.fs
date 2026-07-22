#version 330 core
out vec4 FragColour;
in vec3 ourPosition;

void main ()
{
   FragColour = vec4(ourPosition, 1.0);
}