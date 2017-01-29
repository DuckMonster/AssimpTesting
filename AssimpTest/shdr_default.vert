#version 330 core

struct SomeType {
	vec3 position;
	vec3 normal;
};

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in SomeType a_type;

uniform mat4 u_model;
uniform mat4 u_camera;

out VS_OUT {
	vec3 normal;
	SomeType type;
} vs_out;

void main() {
	gl_Position = u_camera * u_model * vec4(a_vertex, 1.0);
	vs_out.normal = a_normal;
	vs_out.type = a_type;
}