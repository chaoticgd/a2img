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

uniform sampler2D foregroundSampler;
uniform sampler2D backgroundSampler;
uniform sampler2D maskSampler;

varying vec2 uv;

void main()
{
    float foregroundOpacity = texture2D(maskSampler, uv).r;

    gl_FragColor = vec4(0, 0, 0, 0);
    gl_FragColor += texture2D(backgroundSampler, uv) * (1 - foregroundOpacity);
    gl_FragColor += texture2D(foregroundSampler, uv) * foregroundOpacity;
}
