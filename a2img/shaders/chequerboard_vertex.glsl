#version 120

attribute vec3 position;
attribute vec2 vertexUv;

void main() {
    gl_Position.xyz = position;
    gl_Position.w = 1.0;
}
