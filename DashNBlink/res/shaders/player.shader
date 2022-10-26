#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;

out vec2 passed_texture_coords;

uniform float u_id;
uniform vec2 u_room_coords;
uniform mat2 u_rotation;

void main()
{
	vec2 pos = u_rotation * vec2(position.x, position.y);
	pos *= 0.125f;
	pos -= 0.9375;

	vec2 offset = u_room_coords * 0.125f;

	gl_Position = vec4(pos.x + offset.x, pos.y + offset.y, position.z, 1.0f);

	float tex_x = u_id;

	while (tex_x > 16.0f)
	{
		tex_x -= 16.0f;
	}

	float tex_y = floor(u_id / 16.0f);
	vec2 texture_offset = vec2(tex_x, tex_y);
	passed_texture_coords = (textureCoord + texture_offset) * 0.0625f;
};

#shader fragment
#version 330 core

in vec2 passed_texture_coords;

out vec4 out_color;

uniform sampler2D u_texture;

void main()
{
	vec4 texture_color = texture(u_texture, passed_texture_coords);

	out_color = texture_color;
	//out_color = vec4(passed_texture_coords, 0.0f, 1.0f);
};