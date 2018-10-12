#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform float a2_property_intensity;

void main()
{
	vec2 offset = (texture2D(a2_inputSampler, gl_FragCoord.xy / a2_resolution).xy - vec2(0.5, 0.5)) * a2_property_intensity;
	gl_FragColor = texture2D(a2_inputSampler, (gl_FragCoord.xy + offset) / a2_resolution);
}
