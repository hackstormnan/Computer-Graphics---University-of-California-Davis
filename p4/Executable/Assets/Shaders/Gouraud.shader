#shader vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec3 in_normal;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_projection;

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
};

uniform sampler2D u_texture;
uniform Light light;
uniform vec3 u_camPos;

out vec3 out_vertexColor;

void main()
{
	vec3 t_position = vec3(u_world * vec4(in_position, 1.0));
	vec3 t_normal = mat3(transpose(inverse(u_world))) * in_normal;
	vec2 t_texcoord = in_texcoord;

	gl_Position = u_projection * u_view * u_world * vec4(in_position, 1.0);

	//ambient light
	vec3 ambient = light.ambient * texture(u_texture, t_texcoord).rgb;

	//diffuse reflect
	vec3 norm = normalize(t_normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(u_texture, t_texcoord).rgb;

	//specular reflect
	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_camPos - t_position);
	vec3 reflectDir = reflect(light.direction, norm);
	float shiness = 256;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
	vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

	//add up
	out_vertexColor = ambient + diffuse + specular;
}

#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 frag_color;

in vec3 out_vertexColor;

void main()
{
	frag_color = vec4(out_vertexColor, 1.0);
}
