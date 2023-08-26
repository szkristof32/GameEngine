#type vertex
#version 450 core

layout (location = 0) in vec3 in_position;

layout (std140, binding = 0) uniform Push
{
	mat4 Projection;
} push;

void main()
{
	gl_Position = push.Projection * vec4(in_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 out_colour;

void main()
{
	out_colour = vec4(1.0, 1.0, 1.0, 1.0);
}