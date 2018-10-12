#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform int a2_property_radius;
uniform int a2_property_quality;

#pragma include <blurQuality.glsl>
#pragma include <gaussian.glsl>

void main()
{
	gl_FragColor = vec4(0, 0, 0, 0);
	
	// Sample points around the current pixel using a gaussian distribution.
	
	// Find the number of samples requied.
	int numSamples = BQ_COMPUTE_NUM_SAMPLES(a2_property_radius, a2_property_quality);
	float stdDev = numSamples / 6.0;
	float gaussianS = GAUSSIAN_PRECOMPUTE_S(stdDev);
	for(int i = 0; i < numSamples; i++) {
		// Find the position of the sample relative to the current pixel.
		int x = BQ_GET_SAMPLE_COORDINATE(a2_property_radius, numSamples, i);
		vec2 uv = vec2(gl_FragCoord.xy + vec2(x, 0)) / a2_resolution;
		// Find the gaussian distrubtion weighting of the current sample.
		float weight = GAUSSIAN_SAMPLE(stdDev, i - numSamples / 2, gaussianS);
		gl_FragColor += texture2D(a2_inputSampler, uv) * weight;
	}
}
