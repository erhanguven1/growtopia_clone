$input v_color0, v_texcoord0

/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

uniform sampler2D s_tex;
uniform vec4 a_color;

#include <bgfx_shader.sh>

void main()
{
    vec4 texColor = texture2D(s_tex, v_texcoord0);
	gl_FragColor = v_color0 * a_color * texColor;
}