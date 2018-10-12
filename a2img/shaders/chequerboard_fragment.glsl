#version 120

uniform vec2 scrollPosition;
uniform int tileSize;


void main()
{
    vec2 position = floor((scrollPosition + gl_FragCoord.xy) / tileSize);
    float shouldBeShaded = float(mod(position.x + position.y, 2) == 0);
    gl_FragColor = vec4(1, 1, 1, 1);
    gl_FragColor -= vec4(0.2, 0.2, 0.2, 0) * shouldBeShaded;
}
