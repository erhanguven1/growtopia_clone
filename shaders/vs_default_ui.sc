$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

uniform vec4 a_pos;
uniform vec4 a_scale;

/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include <bgfx_shader.sh>

void main()
{
    vec3 scaled_pos = vec3(a_scale.x * a_position.x , a_scale.y * a_position.y, a_position.z);
    vec3 pos = vec3(a_pos.x + scaled_pos.x + 400.0, a_pos.y + scaled_pos.y + 300.0, scaled_pos.z);

    gl_Position = vec4(pos, 1.0);
	v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
}