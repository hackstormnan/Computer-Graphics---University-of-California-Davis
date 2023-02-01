#shader vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec3 in_normal;

out vec3 out_normal;
out vec3 out_position;
out vec2 out_texcoord;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	out_position = vec3(u_world * vec4(in_position, 1.0));
	out_normal = mat3(transpose(inverse(u_world))) * in_normal;
	out_texcoord = in_texcoord;

	gl_Position = u_projection * u_view * u_world * vec4(in_position, 1.0);
}

#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
};

out vec4 frag_color;

in vec3 out_normal;
in vec3 out_position;
in vec2 out_texcoord;

uniform sampler2D u_texture;
uniform Light light;
uniform vec3 u_camPos;

void main()
{
	//ambient light
	vec3 ambient = light.ambient * texture(u_texture, out_texcoord).rgb;

	//diffuse reflect
	vec3 norm = normalize(out_normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(u_texture, out_texcoord).rgb;

	//specular reflect
	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_camPos - out_position);
	vec3 reflectDir = reflect(light.direction, norm);
	float shiness = 128;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
	vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

	//add up
	vec3 result = ambient + diffuse + specular;

	//if (result.x > 0.5) { result.x = 1; }
	//else { result.x = 0; }
	//if (result.y > 0.5) { result.y = 1; }
	//else { result.y = 0; }
	//if (result.z > 0.5) { result.z = 1; }
	//else { result.z = 0; }

	frag_color = vec4(result, 1.0);
}
