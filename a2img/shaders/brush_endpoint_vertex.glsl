#version 120

attribute vec3 position;
attribute vec3 vertexPoint;
varying vec2 point;

void main()
{
    gl_Position.xyz = position;
    gl_Position.w = 1.0;
    point = vertexPoint.xy;
}
