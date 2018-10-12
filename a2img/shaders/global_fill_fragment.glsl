#version 120

uniform sampler2D sampler;
uniform vec2 resolution;
uniform vec2 startingPosition;
uniform float tolerance;

void main() {
    vec4 c1 = texture2D(sampler, startingPosition / resolution);
    vec4 c2 = texture2D(sampler, gl_FragCoord.xy / resolution);

    float distance = sqrt((
        pow(c1.r - c2.r, 2) +
        pow(c1.g - c2.g, 2) +
        pow(c1.b - c2.b, 2) +
        pow(c1.a - c2.a, 2)
    ) / 4);

    gl_FragColor = vec4(0, 0, 0, 1);
    gl_FragColor += vec4(1, 1, 1, 1) * float(distance - tolerance < 0);
}
