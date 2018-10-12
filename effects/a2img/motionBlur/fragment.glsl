#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform int a2_property_radius;
uniform int a2_property_angle;
uniform int a2_property_quality;

#pragma include <blurQuality.glsl>
#pragma include <gaussian.glsl>

void main()
{
	gl_FragColor = vec4(0, 0, 0, 0);
	
	float angle = radians(float(a2_property_angle));
	
	// Find the number of samples required.
	int numSamples = BQ_COMPUTE_NUM_SAMPLES(a2_property_radius, a2_property_quality);
	float stdDev = numSamples / 6.0;
	float gaussianS = GAUSSIAN_PRECOMPUTE_S(stdDev);
	for(int i = 0; i < numSamples; i++) {
		int hyp = BQ_GET_SAMPLE_COORDINATE(a2_property_radius, numSamples, i);
		// Find the position of the sample relative to the current pixel.
		vec2 offset = vec2(hyp * cos(angle), hyp * sin(angle));
		vec2 uv = (gl_FragCoord.xy + offset) / a2_resolution;
		// Find the guassian distribution weight of the sample.
		float weight = GAUSSIAN_SAMPLE(stdDev, i - numSamples / 2, gaussianS);
		gl_FragColor += texture2D(a2_inputSampler, uv) * weight;
	}
}
