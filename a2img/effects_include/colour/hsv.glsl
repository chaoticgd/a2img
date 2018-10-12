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

vec4 hsv_fromRgb(vec4 rgb)
{
    float M = max(max(rgb.r, rgb.g), rgb.b);
    float m = min(min(rgb.r, rgb.g), rgb.b);
    float chroma = M - m;

    float h;
    h += float(M == rgb.r) * mod((rgb.g - rgb.b) / chroma, 6);
    h += float(M == rgb.g) * ((rgb.b - rgb.r) / chroma + 2);
    h += float(M == rgb.b) * ((rgb.r - rgb.g) / chroma + 4);
    h /= 6;

    float s = chroma / M;

    return vec4(h, s, M, rgb.a);
}

vec4 hsv_toRgb(vec4 hsv)
{
    float C = hsv.z * hsv.y;
    hsv.x *= 6;

    float X = C * (1 - abs(mod(hsv.x, 2) - 1));

    vec3 col = vec3(0, 0, 0);
    col += vec3(C, X, 0) * float(0 <= hsv.x && hsv.x <= 1);
    col += vec3(X, C, 0) * float(1 <= hsv.x && hsv.x <= 2);
    col += vec3(0, C, X) * float(2 <= hsv.x && hsv.x <= 3);
    col += vec3(0, X, C) * float(3 <= hsv.x && hsv.x <= 4);
    col += vec3(X, 0, C) * float(4 <= hsv.x && hsv.x <= 5);
    col += vec3(C, 0, X) * float(5 <= hsv.x && hsv.x <= 6);

    float m = hsv.z - C;
    return vec4(col.r + m, col.g + m, col.b + m, hsv.a);
}
