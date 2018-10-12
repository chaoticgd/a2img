#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform int a2_property_hue;
uniform float a2_property_saturation;

#pragma include <colour/hsv.glsl>

void main()
{
	vec4 rgb = texture2D(a2_inputSampler, gl_FragCoord.xy / a2_resolution);
	vec4 hsv = hsv_fromRgb(rgb);
	// Adjust the hue.
	hsv.x += a2_property_hue / 360.0;
	hsv.x = mod(hsv.x, 1);
	// Adjust the saturation.
	hsv.y *= a2_property_saturation + 1;
	gl_FragColor = hsv_toRgb(hsv);
}
