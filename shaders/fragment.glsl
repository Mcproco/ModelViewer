#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

//what texture should we use? think of tex as a id. This id is set via glUniformli(<uniform-location>); You have to bind a texture to that id / texture unit 
uniform sampler2D tex;

void main() {
    FragColor = texture(tex, TexCoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
