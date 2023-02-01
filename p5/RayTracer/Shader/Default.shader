#shader vertex
#version 330 core

uniform vec2 screenPos;

void main()
{
    gl_Position = vec4(screenPos, -1.0, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec3 vertexColor;

void main()
{
	FragColor = vec4(vertexColor, 1.0);
}