#version 120

uniform sampler2D foregroundSampler;
uniform sampler2D backgroundSampler;
uniform sampler2D maskSampler;

varying vec2 uv;

void main()
{
    float foregroundOpacity = texture2D(maskSampler, uv).r;

    gl_FragColor = vec4(0, 0, 0, 0);
    gl_FragColor += texture2D(backgroundSampler, uv) * (1 - foregroundOpacity);
    gl_FragColor += texture2D(foregroundSampler, uv) * foregroundOpacity;
}
