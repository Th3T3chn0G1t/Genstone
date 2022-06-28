layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragment_color;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	fragment_color = color;
}
