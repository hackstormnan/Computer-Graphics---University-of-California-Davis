#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_world * vec4(position, 1.0);
};


#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 frag_color;

uniform vec4 u_color;

void main()
{
	frag_color = u_color;
}
