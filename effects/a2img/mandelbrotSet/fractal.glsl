#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform int a2_property_numIterations;
uniform float a2_property_scale;
uniform float a2_property_positionX;
uniform float a2_property_positionY;
uniform float a2_property_colour;

#pragma include <complex.glsl>

void main()
{
	vec2 offset = vec2(a2_property_positionX, a2_property_positionY);

	// z starts at the origin.
	complex z = complex(0, 0);
	// c starts at the position of the current pixel.
	complex c = (gl_FragCoord.xy - a2_resolution / 2) / pow(a2_property_scale, 2) + offset;
	
	gl_FragColor = vec4(
		0, 0, 0, // Ignored.
		0 // In the set.
	);
	
	for(int i = 0; i < a2_property_numIterations; i++) {
		// Compute an iteration.
		z = complex_pow2(z) + c;
		// Check if z is still in the set.
		if(complex_abs(z) > 2) {
			gl_FragColor = vec4(
				mod(i, 256) / 255,			// Least Significant Bit
				mod(i / 255, 256) / 255,
				mod(i / 65025, 256) / 255,	// Most Significant Bit
				1 // Outside of the set.
			);
			return;
		}
	}
}
