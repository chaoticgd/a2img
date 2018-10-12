/*
	a2img - a free and open source image editor
	Copyright (C) 2018 chaoticgd

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
