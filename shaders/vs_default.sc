$input a_position, a_color0
$output v_color0

uniform vec4 a_pos;

/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include <bgfx_shader.sh>

void main()
{
    vec3 pos = vec3(a_position.x + a_pos.x, a_position.y + a_pos.y, a_position.z);

	gl_Position = mul(u_modelViewProj, vec4(pos, 1.0) );
	v_color0 = a_color0;
}