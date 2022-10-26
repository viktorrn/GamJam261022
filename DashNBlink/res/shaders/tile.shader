#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;

out vec2 passed_texture_coords;

uniform vec2 u_room_coords;

void main()
{
	vec2 pos = vec2(position.x, position.y) * 0.125f - 0.9375;
	vec2 offset = u_room_coords * 0.125f;

	gl_Position = vec4(pos.x + offset.x, pos.y + offset.y, position.z, 1.0f);
	passed_texture_coords = textureCoord;
};

#shader fragment
#version 330 core

in vec2 passed_texture_coords;

out vec4 out_color;

//uniform sampler2D u_texture;

void main()
{
	//vec4 texture_color = texture(u_texture, passed_texture_coords);

	//out_color = texture_color;
	out_color = vec4(passed_texture_coords, 0.0f, 1.0f);
};