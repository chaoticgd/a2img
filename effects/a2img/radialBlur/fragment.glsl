#version 120

uniform sampler2D a2_inputSampler;
uniform vec2 a2_resolution;
uniform int a2_property_radius;
uniform float a2_property_focusPointX;
uniform float a2_property_focusPointY;
uniform int a2_property_innerRadius;
uniform int a2_property_outerRadius;
uniform int a2_property_quality;

#pragma include <blurQuality.glsl>
#pragma include <gaussian.glsl>

void main()
{
	// Get the radius in which the blurring should be maximum.
	float outerRadius = max(float(a2_property_innerRadius), float(a2_property_outerRadius));

	gl_FragColor = vec4(0, 0, 0, 0);
	
	// Get the point in which the blurring should be minimum.
	vec2 focusPoint = vec2(a2_property_focusPointX, a2_property_focusPointY) * a2_resolution;
	vec2 diff = gl_FragCoord.xy - focusPoint;
	// Get the angle between the X axis and the line from the origin to the focus point.
	float angle = atan(diff.y / diff.x);
	float radiusDiff = outerRadius - a2_property_innerRadius;
	
	float blurAmount = clamp((length(diff) - a2_property_innerRadius) / radiusDiff, 0, 1);
	// Determine whether the image should be blurred at all.
	// If it is in the inner radius, it shouldn't be.
	float isInInnerRadius = float(length(diff) < a2_property_innerRadius);
	
	int numSamples = BQ_COMPUTE_NUM_SAMPLES(a2_property_radius, a2_property_quality);
	float stdDev = (numSamples / 6.0) * blurAmount;
	stdDev = max(stdDev, 0.5);
	float gaussianS = GAUSSIAN_PRECOMPUTE_S(stdDev);
	for(int i = 0; i < numSamples; i++) {
		// Find the sample position relative to the current pixel.
		int x = BQ_GET_SAMPLE_COORDINATE(a2_property_radius, numSamples, i);
		vec2 offset = vec2(x * cos(angle), x * sin(angle));
		vec2 uv = (gl_FragCoord.xy + offset) / a2_resolution;
		// Weight the sample according to a guassian distrubtion.
		float weight = GAUSSIAN_SAMPLE(stdDev, i - numSamples / 2, gaussianS);
		gl_FragColor += texture2D(a2_inputSampler, uv) * weight * (1 - isInInnerRadius);
	}
	gl_FragColor += texture2D(a2_inputSampler, gl_FragCoord.xy / a2_resolution) * isInInnerRadius;
}
