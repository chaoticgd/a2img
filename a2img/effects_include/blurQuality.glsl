#define BQ_MINIMUM_SAMPLES(radius) float(3)
#define BQ_MAXIMUM_SAMPLES(radius) float(radius * 2)

#define BQ_COMPUTE_NUM_SAMPLES(radius, quality) \
	int(((BQ_MAXIMUM_SAMPLES(radius) - BQ_MINIMUM_SAMPLES(radius)) / 99) * (quality - 1) + BQ_MINIMUM_SAMPLES(radius))

#define BQ_GET_SAMPLE_COORDINATE(radius, numSamples, i) \
	int(((float(i) / (numSamples - 1)) - 0.5) * radius)