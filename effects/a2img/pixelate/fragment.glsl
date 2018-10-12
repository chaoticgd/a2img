#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform int a2_property_pixelSize;
uniform int a2_property_offsetX;
uniform int a2_property_offsetY;

void main()
{
	vec2 centre = a2_resolution / 2;
	vec2 offset = vec2(a2_property_offsetX, a2_property_offsetY);
	vec2 fragPos = gl_FragCoord.xy + offset - centre;
	// Round the position down.
	vec2 pixelPos = floor(fragPos / float(a2_property_pixelSize)) * a2_property_pixelSize - offset + centre;
	vec2 uv = pixelPos / a2_resolution;
	gl_FragColor = texture2D(a2_inputSampler, uv);
}
