#version 330 core

struct SomeType {
	vec3 position;
	vec3 normal;
};

in VS_OUT {
	vec3 normal;
	SomeType type;
} fs_in;

out vec4 o_color;

void main() {
	o_color = vec4(fs_in.normal, 1.0);
}