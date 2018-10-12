#version 120

uniform sampler2D backgroundSampler;
uniform sampler2D maskSampler;
uniform vec2 resolution;
uniform vec4 brushColour;

void main()
{
    float opacity = texture2D(maskSampler, gl_FragCoord.xy / resolution).r;
    vec4 colour = texture2D(backgroundSampler, gl_FragCoord.xy / resolution);

    gl_FragColor = vec4(0, 0, 0, 0);
    gl_FragColor += colour * (1 - opacity);
    gl_FragColor += colour * brushColour * opacity;
    gl_FragColor.a = colour.a;
}
