layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 fragment_color;

void main() {
	gl_Position = position, 0.0, 1.0;
	fragment_color = color;
}
