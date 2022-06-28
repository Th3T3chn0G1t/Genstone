layout(location = 0) in vec3 fragment_color;

layout(location = 0) out vec4 pixel;

void main() {
	pixel = vec4(fragment_color, 1.0);
}
