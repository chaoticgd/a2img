#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform float a2_property_brightness;
uniform float a2_property_contrast;

void main()
{
	gl_FragColor = texture2D(a2_inputSampler, gl_FragCoord.xy / a2_resolution);
	
	// Add the brightness modifier.
	gl_FragColor.rgb += a2_property_brightness;
	// Adjust the contrast.
	gl_FragColor.rgb = (a2_property_contrast + 1) * (gl_FragColor.rgb - 0.5) + 0.5;
}
