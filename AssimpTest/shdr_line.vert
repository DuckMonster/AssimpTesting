#version 330 core
layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_color;

uniform mat4 u_camera;

out vec3 f_color;

void main() {
	gl_Position = u_camera * vec4(a_vertex, 1.0);
	f_color = a_color;
}