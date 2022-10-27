#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;

out vec2 passed_texture_coords;

uniform float u_offset;

void main()
{
	vec2 pos = vec2(position.x, position.y) * 2.0f;
	vec2 offset = vec2(sin(u_offset / 3.824f), sin(u_offset / -7.487f)) * 0.00005f * u_offset;

	gl_Position = vec4(pos.x + offset.x, pos.y + offset.y, position.z, 1.0f);

	passed_texture_coords = textureCoord;
};

#shader fragment
#version 330 core

in vec2 passed_texture_coords;

out vec4 out_color;

uniform float u_opacity;
uniform sampler2D u_texture0;

void main()
{
	vec4 texture_color = texture(u_texture0, passed_texture_coords);

	texture_color.w *= u_opacity;

	out_color = texture_color;
	//out_color = vec4(passed_texture_coords, 0.0f, 1.0f);
};