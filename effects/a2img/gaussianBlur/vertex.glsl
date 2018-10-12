#version 120

attribute vec3 a2_position;

void main()
{
    gl_Position.xyz = a2_position;
    gl_Position.w = 1.0;
}
