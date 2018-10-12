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

#version 120
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D mask;
varying vec2 uv;
uniform vec2 viewportSize;
uniform float time;

void main()
{
    gl_FragColor = vec4(0.25, 0.25, 1, 0.1 * texture2D(mask, uv).r);

    vec2 offset = 1 / viewportSize;
    vec2 uvNorth = vec2(uv.x, uv.y + offset.y);
    vec2 uvSouth = vec2(uv.x, uv.y - offset.y);
    vec2 uvWest = vec2(uv.x - offset.x, uv.y);
    vec2 uvEast = vec2(uv.x + offset.x, uv.y);
    float isNWEdge = float(
        texture2D(mask, uv).r > 0.5 &&
        (texture2D(mask, uvNorth).r < 0.5 ||
         texture2D(mask, uvWest).r < 0.5));
    float isSEEdge = float(
        texture2D(mask, uv).r > 0.5 &&
         (texture2D(mask, uvSouth).r < 0.5 ||
          texture2D(mask, uvEast).r < 0.5));
    vec2 pos = uv * viewportSize;
    gl_FragColor.a += isNWEdge + isSEEdge;

    float timeVal = time * (isSEEdge - isNWEdge);

    float isEven = float(mod(int(pos.x) + int(pos.y) + timeVal, 8) > 3);
    gl_FragColor.rgb += vec2((isNWEdge + isSEEdge) * isEven, 0).xxx;
    gl_FragColor.rgb -= vec2((1 - (isNWEdge + isSEEdge)) * isEven, 0).xxx;
}
