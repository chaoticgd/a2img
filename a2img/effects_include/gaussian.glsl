#pragma include <constants.glsl>

#define GAUSSIAN_INTERNAL_MULT(stdDev, s) \
	(1 / sqrt(C_PI * s))

#define GAUSSIAN_INTERNAL_EXPONENT(stdDev, s, x) \
	(-pow(x, 2) / s)

#define GAUSSIAN_PRECOMPUTE_S(stdDev) \
	(2 * pow(stdDev, 2.0))

#define GAUSSIAN_SAMPLE(stdDev, x, s) \
	(GAUSSIAN_INTERNAL_MULT(stdDev, s) * \
	exp(GAUSSIAN_INTERNAL_EXPONENT(stdDev, s, float(x))))

// Inefficient implementation. Use at your own risk.
/*float gaussian_sample(float stdDev, float x)
{
    float s = 2 * pow(stdDev, 2.0);
    float mult = 1 / sqrt(C_PI * s);
    float exponent = -pow(x, 2) / s;
    return mult * exp(exponent);
}
*/