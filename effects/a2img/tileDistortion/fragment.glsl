#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform float a2_property_tileSize;

void main()
{
	vec2 offset = sin(gl_FragCoord.xy / a2_property_tileSize) * a2_property_tileSize;
	gl_FragColor = texture2D(a2_inputSampler, (gl_FragCoord.xy + offset) / a2_resolution);
}
