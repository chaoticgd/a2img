#version 120

uniform float diameter;
varying vec2 point;

void main()
{
    gl_FragColor = vec4(1, 1, 1, 1) * float(
        distance(gl_FragCoord.xy, point) < diameter / 2.0
    );
}
