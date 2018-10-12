#version 120

uniform sampler2D a2_inputSampler;
uniform sampler2D a2_stageSamplers[1];
uniform vec2 a2_resolution;
uniform int a2_property_numIterations;
uniform float a2_property_scale;
uniform float a2_property_positionX;
uniform float a2_property_positionY;
uniform float a2_property_colour;

void main()
{
    vec4 status = texture2D(a2_stageSamplers[0], gl_FragCoord.xy / a2_resolution);
    int numIters = int(mod(status.x * 255, 6));
    float isOutOfSet = status.w;

    // Specify the different colours that will appear in the result.

    gl_FragColor = vec4(0, 0, 0, 1);
	
    gl_FragColor += vec4(1, 0, 0, 0) * float(numIters == 0) * isOutOfSet;
    gl_FragColor += vec4(1, 1, 0, 0) * float(numIters == 1) * isOutOfSet;
    gl_FragColor += vec4(0, 1, 0, 0) * float(numIters == 2) * isOutOfSet;
    gl_FragColor += vec4(0, 1, 1, 0) * float(numIters == 3) * isOutOfSet;
    gl_FragColor += vec4(0, 0, 1, 0) * float(numIters == 4) * isOutOfSet;
    gl_FragColor += vec4(1, 0, 1, 0) * float(numIters == 5) * isOutOfSet;
}
